! 
! Motif
!
! Copyright (c) 1987-2012, The Open Group. All rights reserved.
!
! These libraries and programs are free software; you can
! redistribute them and/or modify them under the terms of the GNU
! Lesser General Public License as published by the Free Software
! Foundation; either version 2 of the License, or (at your option)
! any later version.
!
! These libraries and programs are distributed in the hope that
! they will be useful, but WITHOUT ANY WARRANTY; without even the
! implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
! PURPOSE. See the GNU Lesser General Public License for more
! details.
!
! You should have received a copy of the GNU Lesser General Public
! License along with these librararies and programs; if not, write
! to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
! Floor, Boston, MA 02110-1301 USA
! 
! 
! HISTORY
! 
!   $XConsortium: BBoard.msg /main/9 1995/07/13 20:41:45 drk $
!OSF/Motif: @(#)BBoard.msg	3.4  92/01/31
!
!  
!  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
!  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
!  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
!  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
!  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
!  SOFTWARE IS HEREBY TRANSFERRED.
!  
!  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
!  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
!  
!  
!  
!  
!  
!  
!  
!  
!  
! DEC is a registered trademark of Digital Equipment Corporation
! DIGITAL is a registered trademark of Digital Equipment Corporation
!
 value
        message : "The following is a Bulletin Board with\nthe following characteristics:\n\n" &
	"   - border width is 5\n   - background color is Coral (using RGB())\n " &
	"  - dialogTitle is BBoard Test\n " &
	"  - marginWidth is 50\n   - marginHeight is 50\n   - noResize is true\n   - default" &
	"Position is false\n" &
	"   - A label with a labelPixmap of a palette\n     is a child of the Bulletin Board\n" &
	"   - Default and Cancel buttons" &
	"\nAll other resources are set by default.\n" &
	"You can perform a variety of interesting\n" &
	"   tests of the button and parent visuals\n" &
	"   by clicking on and traversing between the\n" &
	"   Default and Cancel buttons and by changing\n" &
	"   the BulletinBoard's background color\n" &
	"   using the button so labelled.";


