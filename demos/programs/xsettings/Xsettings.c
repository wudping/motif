/**
 *
 * $Id: Xsettings.c,v 1.2 2009/06/18 20:05:55 rwscott Exp $
 *
 * Copyright 2009 Rick Scott <rwscott@users.sourceforge.net>
 *
 **/
/*
Portions of this software are derived from the xsettings reference
implementation, found at:

http://www.freedesktop.org/software/xsettings/releases/xsettings-0.2.tar.gz

It contains the following copright notice ...

 * Copyright © 2001 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Red Hat not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Red Hat makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL RED HAT
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Owen Taylor, Red Hat, Inc.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <limits.h>

#include "XsettingsP.h"
#include <X11/Xmd.h>
#include <Xm/XmP.h>

/*
   Widget methods, forward declarations
 */

static void class_initialize(void);
static void class_part_initialize(WidgetClass widget_class);
static void initialize(Widget request, Widget new_w, ArgList args, Cardinal *num_args);
static void destroy(Widget w);
static Boolean set_values(Widget old, Widget request, Widget new_w, ArgList args, Cardinal *num_args);
static void get_values_hook(Widget w, ArgList args, Cardinal *num_args);

/*
   Widget default resources
 */

#define Offset(field) XtOffsetOf(XmXsettingsRec, xsettings.field)
static XtResource resources[] =
{
    {
	XmNnumXsettings, XmCNumXsettings, XtRInt,
	sizeof(int), Offset(num_settings),
	XtRImmediate, (XtPointer)0
    },
    {
	XmNxsettings, XmCXsettings, XtRPointer,
	sizeof(XtPointer), Offset(setting),
	XtRImmediate, NULL
    },
    {
	XmNxsettingsCallback, XtCCallback, XtRCallback,
	sizeof(XtCallbackList), Offset(xsettings_callback),
	XtRCallback, NULL
    },
    {
	XmNuserData, XmCUserData, XmRPointer,
	sizeof(XtPointer), Offset(user_data),
	XtRImmediate, NULL
    },
};
#undef Offset

/*
   Widget class record
 */
/* *INDENT-OFF* */
XmXsettingsClassRec xmXsettingsClassRec = {
	/* Object Class Part */
	{
/* pointer to superclass ClassRec   WidgetClass       */ (WidgetClass) &objectClassRec,
/* widget resource class name       String            */ "XmXsettings",
/* size in bytes of widget record   Cardinal          */ sizeof(XmXsettingsRec),
/* class initialization proc        XtProc            */ class_initialize,
/* dynamic initialization           XtWidgetClassProc */ class_part_initialize,
/* has class been initialized?      XtEnum            */ False,
/* initialize subclass fields       XtInitProc        */ initialize,
/* notify that initialize called    XtArgsProc        */ NULL,
/* NULL                             XtProc            */ NULL,
/* NULL                             XtPointer         */ NULL,
/* NULL                             Cardinal          */ (Cardinal)NULL,
/* resources for subclass fields    XtResourceList    */ resources,
/* number of entries in resources   Cardinal          */ XtNumber(resources),
/* resource class quarkified        XrmClass          */ NULLQUARK,
/* NULL                             Boolean           */ (Boolean)NULL,
/* NULL                             XtEnum            */ (XtEnum)NULL,
/* NULL				    Boolean           */ (Boolean)NULL,
/* NULL                             Boolean           */ (Boolean)NULL,
/* free data for subclass pointers  XtWidgetProc      */ destroy,
/* NULL                             XtProc            */ NULL,
/* NULL			            XtProc            */ NULL,
/* set subclass resource values     XtSetValuesFunc   */ set_values,
/* notify that set_values called    XtArgsFunc        */ NULL,
/* NULL                             XtProc            */ NULL,
/* notify that get_values called    XtArgsProc        */ get_values_hook,
/* NULL                             XtProc            */ NULL,
/* version of intrinsics used       XtVersionType     */ XtVersion,
/* list of callback offsets         XtPointer         */ NULL,
/* NULL                             String            */ NULL,
/* NULL                             XtProc            */ NULL,
/* NULL                             XtProc            */ NULL,
/* pointer to extension record      XtPointer         */ NULL
	},
	/* Xsettings Class Part */
	{
		NULL
	}
};
/* *INDENT-ON* */

WidgetClass xmXsettingsWidgetClass = (WidgetClass)&xmXsettingsClassRec;

static int
ignore_errors (Display *display, XErrorEvent *event)
{
  return True;
}

#if 1
typedef enum
{
  XSETTINGS_SUCCESS,
  XSETTINGS_NO_MEM,
  XSETTINGS_ACCESS,
  XSETTINGS_FAILED,
  XSETTINGS_NO_ENTRY,
  XSETTINGS_DUPLICATE_ENTRY
} XSettingsResult;

typedef struct _XSettingsBuffer
{
  char byte_order;
  size_t len;
  unsigned char *data;
  unsigned char *pos;
} XSettingsBuffer;

#define BYTES_LEFT(buffer) ((buffer)->data + (buffer)->len - (buffer)->pos)
#define XSETTINGS_PAD(n,m) ((n + m - 1) & (~(m-1)))

static XSettingsResult
fetch_card8 (XmXsettingsWidget xs,
             XSettingsBuffer *buffer,
	     CARD8           *result)
{
  if (BYTES_LEFT (buffer) < 1)
    return XSETTINGS_ACCESS;

  *result = *(CARD8 *)buffer->pos;
  buffer->pos += 1;

  return XSETTINGS_SUCCESS;
}

static XSettingsResult
fetch_card16 (XmXsettingsWidget xs,
              XSettingsBuffer *buffer,
	      CARD16          *result)
{
  CARD16 x;

  if (BYTES_LEFT (buffer) < 2)
    return XSETTINGS_ACCESS;

  x = *(CARD16 *)buffer->pos;
  buffer->pos += 2;
  
  if (buffer->byte_order == xs->xsettings.local_byte_order)
    *result = x;
  else
    *result = (x << 8) | (x >> 8);

  return XSETTINGS_SUCCESS;
}

static XSettingsResult
fetch_ushort (XmXsettingsWidget xs,
              XSettingsBuffer *buffer,
	      unsigned short  *result) 
{
  CARD16 x;
  XSettingsResult r;  

  r = fetch_card16 (xs, buffer, &x);
  if (r == XSETTINGS_SUCCESS)
    *result = x;

  return r;
}

static XSettingsResult
fetch_card32 (XmXsettingsWidget xs,
              XSettingsBuffer *buffer,
	      CARD32          *result)
{
  CARD32 x;

  if (BYTES_LEFT (buffer) < 4)
    return XSETTINGS_ACCESS;

  x = *(CARD32 *)buffer->pos;
  buffer->pos += 4;
  
  if (buffer->byte_order == xs->xsettings.local_byte_order)
    *result = x;
  else
    *result = (x << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);
  
  return XSETTINGS_SUCCESS;
}

static char
xsettings_byte_order (void)
{
  CARD32 myint = 0x01020304;
  return (*(char *)&myint == 1) ? MSBFirst : LSBFirst;
}
#endif

static void
apply_theme(XmXsettingsWidget xs, String theme_name)
{
    /* themes/theme_name/Xm/xrdb.ad */
    printf("%s:%s(%d) - %s %s \"%s\"\n",
    	__FILE__, __FUNCTION__, __LINE__,
    	XtName(XtParent((Widget)xs)),
    	XtName((Widget)xs),
    	theme_name);
}

static void
parse_settings(XmXsettingsWidget xs, unsigned char *data, size_t len)
{
XSettingsBuffer buffer;
XSettingsResult result = XSETTINGS_SUCCESS;
int i;

    /*
    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    */

    buffer.pos = buffer.data = data;
    buffer.len = len;
    result = fetch_card8(xs, &buffer, (CARD8 *)&buffer.byte_order);
    if (buffer.byte_order != MSBFirst && buffer.byte_order != LSBFirst)
    {
	fprintf (stderr, "Invalid byte order in XSETTINGS property\n");
	result = XSETTINGS_FAILED;
	goto out;
    }
    buffer.pos += 3;

    result = fetch_card32 (xs, &buffer, (CARD32 *)&xs->xsettings.serial);
    if (result != XSETTINGS_SUCCESS) goto out;

    /*
    printf("%s:%s(%d) - serial %i\n",
    	__FILE__, __FUNCTION__, __LINE__,
    	xs->xsettings.serial);
    	*/

    result = fetch_card32 (xs, &buffer, (CARD32 *)&xs->xsettings.num_settings);
    if (result != XSETTINGS_SUCCESS) goto out;
    /*
    printf("%s:%s(%d) - num_settings %i\n",
    	__FILE__, __FUNCTION__, __LINE__,
    	xs->xsettings.num_settings);
    	*/

    xs->xsettings.setting = (XSettingsSetting *)XtCalloc(xs->xsettings.num_settings, sizeof(XSettingsSetting));

    for (i = 0; i < xs->xsettings.num_settings; i++)
    {
      char *name;
      char *v_string;
      CARD16 name_len;
      size_t pad_len;
      CARD32 v_int;
      
      result = fetch_card8 (xs, &buffer, (CARD8 *)&xs->xsettings.setting[i].type);
      if (result != XSETTINGS_SUCCESS) goto out;

      buffer.pos += 1;

      result = fetch_card16 (xs, &buffer, &name_len);
      if (result != XSETTINGS_SUCCESS) goto out;

      pad_len = XSETTINGS_PAD(name_len, 4);
      if (BYTES_LEFT (&buffer) < pad_len)
	{
	  result = XSETTINGS_ACCESS;
	  goto out;
	}

      name = XtMalloc(name_len + 1);

      memcpy (name, buffer.pos, name_len);
      name[name_len] = '\0';
      buffer.pos += pad_len;
      xs->xsettings.setting[i].name = XrmStringToQuark(name);
      XtFree(name);
      name = NULL;

      result = fetch_card32 (xs, &buffer, &v_int);
      xs->xsettings.setting[i].last_change_serial = v_int;
      if (result != XSETTINGS_SUCCESS) goto out;
    /*
    fprintf(stderr, "%s:%s(%d) - name_len %i \"%s\" %li ",
    	__FILE__, __FUNCTION__, __LINE__,
    	name_len,
	XrmQuarkToString(xs->xsettings.setting[i].name),
	xs->xsettings.setting[i].last_change_serial);
	*/

      switch (xs->xsettings.setting[i].type)
	{
	case XSETTINGS_TYPE_INT:
	  result = fetch_card32 (xs, &buffer, (CARD32 *)&xs->xsettings.setting[i].data.v_int);
	  if (result != XSETTINGS_SUCCESS) goto out;
	  /*
	  fprintf(stderr, "%i", xs->xsettings.setting[i].data.v_int);
	  */
	  break;
	case XSETTINGS_TYPE_STRING:
	  result = fetch_card32 (xs, &buffer, &v_int);
	  if (result != XSETTINGS_SUCCESS) goto out;

	  pad_len = XSETTINGS_PAD (v_int, 4);
	  if (v_int + 1 == 0 || /* Guard against wrap-around */
	      BYTES_LEFT (&buffer) < pad_len)
	    {
	      result = XSETTINGS_ACCESS;
	      goto out;
	    }

	  v_string = XtMalloc (v_int + 1);
	  
	  memcpy (v_string, buffer.pos, v_int);
	  v_string[v_int] = '\0';
	  buffer.pos += pad_len;
	  xs->xsettings.setting[i].data.v_string = XrmStringToQuark(v_string);
	  XtFree(v_string);
	  /*
	  fprintf(stderr, "\"%s\"", XrmQuarkToString(xs->xsettings.setting[i].data.v_string));
	  */
	  break;
	case XSETTINGS_TYPE_COLOR:
	  result = fetch_ushort (xs, &buffer, &xs->xsettings.setting[i].data.v_color.red);
	  if (result != XSETTINGS_SUCCESS) goto out;
	  result = fetch_ushort (xs, &buffer, &xs->xsettings.setting[i].data.v_color.green);
	  if (result != XSETTINGS_SUCCESS) goto out;
	  result = fetch_ushort (xs, &buffer, &xs->xsettings.setting[i].data.v_color.blue);
	  if (result != XSETTINGS_SUCCESS) goto out;
	  result = fetch_ushort (xs, &buffer, &xs->xsettings.setting[i].data.v_color.alpha);
	  if (result != XSETTINGS_SUCCESS) goto out;
	  /*
	  fprintf(stderr, "0x%04x%04x%04x %04x",
	      xs->xsettings.setting[i].data.v_color.red,
	      xs->xsettings.setting[i].data.v_color.green,
	      xs->xsettings.setting[i].data.v_color.blue,
	      xs->xsettings.setting[i].data.v_color.alpha);
	      */

	  break;
	default:
	  /* Quietly ignore unknown types */
	  break;
	}
	/*
	fprintf(stderr, "\n");
	*/
    }

out:
    return;
}

static void
read_settings(XmXsettingsWidget xs)
{
Cardinal num_old_settings;
XSettingsSetting *old_setting;
int (*old_handler) (Display *, XErrorEvent *);
int result;
Atom type;
int format;
unsigned long n_items;
unsigned long bytes_after;
unsigned char *data;

    /*
    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    */

    num_old_settings = xs->xsettings.num_settings;
    old_setting = xs->xsettings.setting;
    xs->xsettings.num_settings = 0;
    xs->xsettings.setting = NULL;

    old_handler = XSetErrorHandler(ignore_errors);
    result = XGetWindowProperty(XtDisplay((Widget)xs), xs->xsettings.manager,
	xs->xsettings.atom, 0, LONG_MAX,
	False, xs->xsettings.atom,
	&type, &format, &n_items, &bytes_after, &data);
    XSetErrorHandler(old_handler);
    if (result == Success && type == xs->xsettings.atom)
    {
    	if (format == 8)
    	{
    	int old_iter = 0, new_iter = 0;
	XmXsettingsCallbackStruct cbs;
	XrmQuark multiclick_time;
	XrmQuark theme_name;

	    /*
	    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
	    */
	    parse_settings(xs, data, n_items);
	    multiclick_time = XrmPermStringToQuark("Net/DoubleClickTime");
	    theme_name = XrmPermStringToQuark("Net/ThemeName");
	    cbs.action = XSETTINGS_ACTION_NONE;
	    while (old_iter < num_old_settings || new_iter < xs->xsettings.num_settings)
	    {
	    int cmp;

		/*
		fprintf(stderr, "%s:%s(%d) - %i(%i) %i(%i)\n",
		    __FILE__, __FUNCTION__, __LINE__,
		    old_iter, num_old_settings,
		    new_iter, xs->xsettings.num_settings);
		    */
		if (old_iter < num_old_settings && new_iter < xs->xsettings.num_settings)
		{
		    cmp = old_setting[old_iter].name == xs->xsettings.setting[new_iter].name ? 0 : -1;
		}
		else if (old_iter < num_old_settings)
		{
		    cmp = -1;
		}
		else
		{
		    cmp = 1;
		}
		if (cmp < 0)
		{
		    /*
		    fprintf(stderr, "%s:%s(%d) - %s Deleted\n",
			__FILE__, __FUNCTION__, __LINE__,
			XrmQuarkToString(old_setting[old_iter].name));
			*/
		    cbs.action = XSETTINGS_ACTION_DELETED;
		    cbs.setting = old_setting[old_iter];
		}
		else if (cmp == 0)
		{
		    if (old_setting[old_iter].last_change_serial != xs->xsettings.setting[new_iter].last_change_serial)
		    {
			/*
			fprintf(stderr, "%s:%s(%d) - %s %li %li Changed\n",
			    __FILE__, __FUNCTION__, __LINE__,
			    XrmQuarkToString(old_setting[old_iter].name),
			    old_setting[old_iter].last_change_serial, xs->xsettings.setting[new_iter].last_change_serial);
			    */
			cbs.action = XSETTINGS_ACTION_CHANGED;
			cbs.setting = xs->xsettings.setting[new_iter];
		    }
		}
		else
		{
		    /*
		    fprintf(stderr, "%s:%s(%d) - %s New\n",
			__FILE__, __FUNCTION__, __LINE__,
			XrmQuarkToString(xs->xsettings.setting[new_iter].name));
			*/
		    cbs.action = XSETTINGS_ACTION_NEW;
		    cbs.setting = xs->xsettings.setting[new_iter];
		}
		if (cbs.action != XSETTINGS_ACTION_NONE)
		{
		    if (multiclick_time == xs->xsettings.setting[new_iter].name)
		    {
			XtSetMultiClickTime(XtDisplay((Widget)xs), xs->xsettings.setting[new_iter].data.v_int);
		    }
		    else if (theme_name == xs->xsettings.setting[new_iter].name)
		    {
			apply_theme(xs, XrmQuarkToString(xs->xsettings.setting[new_iter].data.v_string));
		    }
		    XtCallCallbackList((Widget)xs, xs->xsettings.xsettings_callback, &cbs);
		    cbs.action = XSETTINGS_ACTION_NONE;
		}
		old_iter++;
		new_iter++;
	    }
    	}
    	else
    	{
	    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    	}
    }
    else
    {
	printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    }
    XtFree((char *)old_setting);
}

static void
xsetting_callback(Widget parent, XtPointer client_data, XEvent *event, Boolean *continue_to_dispatch)
{
XmXsettingsWidget xs = (XmXsettingsWidget)client_data;

    /*
    printf("%s:%s(%d) - %s %s\n",
	__FILE__, __FUNCTION__, __LINE__,
	parent ? XtName(parent) : "NULL",
	xs ? XtName((Widget)xs) : "NULL");
	*/
    if (event && event->xany.window == xs->xsettings.manager)
    {
	read_settings(xs);
    }
}

static void 
class_initialize(void)
{
    /*
    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    */
}

static void 
class_part_initialize(WidgetClass widget_class)
{
    /*
    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    */
}

static void 
initialize(Widget request, Widget new_w, ArgList args, Cardinal *num_args)
{
String selection_atom_name = "";
int selection_atom_name_len;
XmXsettingsWidget new_xs = (XmXsettingsWidget)new_w;
String params[] = {"param 1", "param 2"};
Cardinal num_params = XtNumber(params);

    /*
    printf("%s:%s(%d) - %s -> %s\n", __FILE__, __FUNCTION__, __LINE__,
    	XtName(XtParent(new_w)),
    	XtName(new_w));
    	*/

    new_xs->xsettings.num_settings = 0;
    new_xs->xsettings.setting = NULL;
    new_xs->xsettings.serial = 0;
    new_xs->xsettings.local_byte_order = xsettings_byte_order();
    selection_atom_name_len = snprintf(selection_atom_name, 0, "_XSETTINGS_S%d", XScreenNumberOfScreen(XtScreen(new_w)));
    if (selection_atom_name_len > 0)
    {
	selection_atom_name_len++;
	selection_atom_name = XtMalloc(selection_atom_name_len);
	if (selection_atom_name > 0)
	{
	Atom selection_atom;

	    snprintf(selection_atom_name, selection_atom_name_len, "_XSETTINGS_S%d", XScreenNumberOfScreen(XtScreen(new_w)));
	    /*
	    printf("%s:%s(%d) - %s -> %s \"%s\"\n", __FILE__, __FUNCTION__, __LINE__,
		XtName(XtParent(new_w)),
		XtName(new_w),
		selection_atom_name);
		*/
	    selection_atom = XInternAtom(XtDisplay(new_w), selection_atom_name, True);
	    XtFree(selection_atom_name);
	    if (selection_atom != None)
	    {
		/*
		printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
		*/
		XGrabServer(XtDisplay(new_w));
		new_xs->xsettings.manager = XGetSelectionOwner(XtDisplay(new_w), selection_atom);
		if (new_xs->xsettings.manager != None)
		{
		    /*
		    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
		    */
		    XSelectInput(XtDisplay(new_w), new_xs->xsettings.manager, PropertyChangeMask | StructureNotifyMask);
		    XtRegisterDrawable(XtDisplay(new_w), new_xs->xsettings.manager, XtParent(new_w));
		    XtAddEventHandler(XtParent(new_w), PropertyChangeMask | StructureNotifyMask, False, xsetting_callback, new_w);
		    new_xs->xsettings.atom = XInternAtom(XtDisplay(new_w), "_XSETTINGS_SETTINGS", True);
		    read_settings(new_xs);
		}
		else
		{
		    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
		}
		XUngrabServer(XtDisplay(new_w));
	    }
	    else
	    {
		printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
	    }
	}
	else
	{
	    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
	}
    }
    else
    {
	printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    }
}

static void 
destroy(Widget w)
{
XmXsettingsWidget xs = (XmXsettingsWidget)w;

    /*
    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    */
    if (xs->xsettings.setting)
    {
	XtFree((char *)xs->xsettings.setting);
    }
}

static Boolean 
set_values(Widget old, Widget request, Widget new_w, ArgList args, Cardinal *num_args)
{
XmXsettingsWidget old_xs = (XmXsettingsWidget) old;
XmXsettingsWidget new_xs = (XmXsettingsWidget) new_w;

    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    if (old_xs->xsettings.num_settings != new_xs->xsettings.num_settings)
    {
	XtAppWarningMsg(XtWidgetToApplicationContext(new_w),
	    XrmQuarkToString(new_w->core.xrm_name),
	    "Invalid parameter",
	    new_w->core.widget_class->core_class.class_name,
	    "XmN" XmNnumXsettings " is read-only",
	    NULL, 0);
	new_xs->xsettings.num_settings = old_xs->xsettings.num_settings;
    }
    if (old_xs->xsettings.setting != new_xs->xsettings.setting)
    {
	XtAppWarningMsg(XtWidgetToApplicationContext(new_w),
	    XrmQuarkToString(new_w->core.xrm_name),
	    "Invalid parameter",
	    new_w->core.widget_class->core_class.class_name,
	    "XmN" XmNxsettings " is read-only",
	    NULL, 0);
	XtFree((char *)new_xs->xsettings.setting);
	new_xs->xsettings.setting = old_xs->xsettings.setting;
    }
    return(False);
}

static void 
get_values_hook(Widget w, ArgList args, Cardinal *num_args)
{
    /*
    printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
    */
}

Widget
XmCreateXsettings(Widget parent, String name, ArgList arglist, Cardinal argcount)
{
    return(XtCreateWidget(name, xmXsettingsWidgetClass, parent, arglist, argcount));
}
