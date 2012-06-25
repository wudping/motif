/*
 * Motif
 *
 * Copyright (c) 1987-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these librararies and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 * 
 */

#ifndef _XmExt18List_h
#define _XmExt18List_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#include <Xm/Ext.h>

/************************************************************
*	INCLUDE FILES
*************************************************************/

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#define XmANY_COLUMN	-1

enum { XmEXT18LIST_FOUND, XmEXT18LIST_NOT_FOUND };

typedef struct _XmExt18ListClassRec	*XmExt18ListWidgetClass;
typedef struct _XmExt18ListRec		*XmExt18ListWidget;

typedef struct _Xm18RowInfo {
    /*
     * Used by the XmIList widget.
     */
    XmString * values;		/* The array of column strings */
    Pixmap pixmap;		/* the mini-icon pixmaps. */
    Boolean selected;		/* Is this row selected. */

    /*
     * Provided for the convience of the application programmer.
     */
    short *sort_id;
    XtPointer data;

    /*
     * Private to the XmIList widget (do not modify these).
     */
    short pix_width;		/* width of the pixmap. */
    short pix_height;		/* height of the pixmap. */
    short height;		/* height of the row */
    Boolean old_sel_state;	/* previous select state. */
    short pix_depth;		/* height of the pixmap. */
} Xm18RowInfo;

typedef struct _Xm18ExtListCallbackStruct {
    int reason;		/* Why was callback called? */
    XEvent *event;	/* The X Event associated with find button press... */
    String string;	/* The search string used to do find */
    int column;		/* The column index into row values */
    Xm18RowInfo *row;	/* The row info structure of the matching row */
} XmExt18ListCallbackStruct;

typedef int (*Xm18SortFunction)(
#ifndef _NO_PROTO
short, Xm18RowInfo *, Xm18RowInfo *
#endif
);

/************************************************************
*	MACROS
*************************************************************/

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/* 
 * Function Name: XmExt18ListGetSelectedRows
 * Description:   Takes an Extended List and returns a NULL terminated array
 *                of pointers to selected rows from the internal list
 * Arguments:     w - the extended list widget
 * Returns:       Xm18RowInfo **
 */

Xm18RowInfo ** XmExt18ListGetSelectedRows(
#ifndef _NO_PROTO
Widget
#endif
);

/*	Function Name: XmCreateExtended18List
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The Widget created.
 */

Widget XmCreateExtended18List(
#ifndef _NO_PROTO
Widget, String, ArgList, Cardinal
#endif
);

Widget XmCreateExt18List(
#ifndef _NO_PROTO
Widget, String, ArgList, Cardinal
#endif
);

extern Widget XmVaCreateExt18List(
                        Widget parent,
                        char *name,
                        ...);

extern Widget XmVaCreateManagedExt18List(
                        Widget parent,
                        char *name,
                        ...);

/*  Function Name: XmExt18ListUnselectAllItems
 *  Description:   Unselects all rows
 *  Arguments:     w - the ilist widget.
 *  Returns:       none
 */
void
XmExt18ListUnselectAllItems( 
#ifndef _NO_PROTO
Widget 
#endif
);

/*  Function Name: XmExt18ListUnselectItem
 *  Description:   Unselects the row passed in
 *  Arguments:     w - the ilist widget.
 *                 row_info - ptr to the row passed in
 *  Returns:       none
 */
void
XmExt18ListUnselectItem( 
#ifndef _NO_PROTO
Widget, Xm18RowInfo *
#endif
);

/*  Function Name: XmExt18ListToggleRow
 *  Description:   Toggles the selection state of a specified row
 *  Arguments:     w - the extended list widget
 *  Returns:       none
 */
extern void 
XmExt18ListToggleRow(
#ifndef _NO_PROTO
Widget, short
#endif
);

/*  Function Name: XmExt18ListSelectItems
 *  Description:   Set selection state by matching column entries to XmString
 *  Arguments:     w - the extended list widget
 *		   item - XmString to use as selection key
 *		   column - column number (0 - N) to match (or XmANY_COLUMN)
 *		   notify - if True, call XmNsingleSelectionCallback
 *  Returns:       none
 */
extern void
XmExt18ListSelectItems(
#ifndef _NO_PROTO
Widget, XmString, int, Boolean
#endif
);

/*  Function Name: XmExt18ListDeselectItems
 *  Description:   Set selection state by matching column entries to XmString
 *  Arguments:     w - the extended list widget
 *		   item - XmString to use as selection key
 *		   column - column number (0 - N) to match (or XmANY_COLUMN)
 *  Returns:       none
 */
extern void
XmExt18ListDeselectItems(
#ifndef _NO_PROTO
Widget, XmString, int
#endif
);

/*  Function Name: XmExt18ListSelectAllItems
 *  Description:   Set selection state on all rows
 *  Arguments:     w - the extended list widget
 *		   notify - if True, call XmNsingleSelectionCallback for each
 *  Returns:       none
 */
extern void
XmExt18ListSelectAllItems(
#ifndef _NO_PROTO
Widget, Boolean
#endif
);

/*  Function Name: XmExt18ListSelectRow
 *  Description:   Set selection state on all rows
 *  Arguments:     w - the extended list widget
 *		   row - the row to select
 *		   notify - if True, call XmNsingleSelectionCallback
 *  Returns:       none
 */
extern void
XmExt18ListSelectRow(
#ifndef _NO_PROTO
Widget, int, Boolean
#endif
);

/*  Function Name: XmExt18ListDeselectRow
 *  Description:   Set selection state on all rows
 *  Arguments:     w - the extended list widget
 *		   row - the row to select
 *  Returns:       none
 */
extern void
XmExt18ListDeselectRow(
#ifndef _NO_PROTO
Widget, int
#endif
);

/* 
 * Function Name: XmExt18ListGetSelectedRowArray
 * Description:   Takes an Extended List and returns a NULL terminated array
 *                of pointers to selected rows from the internal list
 * Arguments:     w - the extended list widget
 *		  num_rows - pointer to the number of rows
 * Returns:       array of integer (selected) row numbers
 */
extern int *
XmExt18ListGetSelectedRowArray(
#ifndef _NO_PROTO
Widget, int *
#endif
);

/*  Function Name: XmExt18ListMakeRowVisible
 *  Description:   Shifts the visible extended list rows as desired
 *  Arguments:     w - the extended list widget
 *		   row - the row number wished to be made visible
 *  Returns:       none
 */
void
XmExt18ListMakeRowVisible(
#ifndef _NO_PROTO
Widget, int
#endif
);

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

extern WidgetClass xmExt18ListWidgetClass;

#if defined(__cplusplus)
}
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmExt18List_h */
