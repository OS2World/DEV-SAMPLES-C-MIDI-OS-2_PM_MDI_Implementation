// Guard Against Multiple Inclusion
// --------------------------------
   #if !defined(MDI_H)
      #define MDI_H

// Constants
// ---------
   #define MDI_DESKTOPCLASS             "MDIDesktop"
   #define MDI_DOCUMENTCLASS            "MDIDocument"
   #define MDI_MODNAME                  "MDI.DLL"


// Messages (can be sent to both the desktop and document windows)
// ---------------------------------------------------------------
   #define MDI_MSG_LOCATE_DESKTOP_DATA    WM_USER+1
                                        // mp1 : NULL
                                        // mp2 : NULL
                                        // Returns PVOID to desktop data

   #define MDI_MSG_LOCATE_DOCUMENT_DATA   WM_USER+2
                                        // mp1 : NULL
                                        // mp2 : NULL
                                        // Returns PVOID to document data
                                        //  (currently active document if sent to desktop window)

   #define MDI_MSG_LOCATE_INSTANCE_DATA   WM_USER+3
                                        // mp1 : NULL
                                        // mp2 : NULL
                                        // Returns PVOID to document instance data
                                        //  (currently active document if sent to desktop window)

   #define MDI_MSG_LOCATE_ACTIVE_DOCUMENT WM_USER+4
                                        // mp1 : NULL
                                        // mp2 : NULL
                                        // Returns HWND of active document
                                        //  (current document if sent to document window)

   #define MDI_MSG_LOCATE_DESKTOP_MENU    WM_USER+5
                                        // mp1 : NULL
                                        // mp2 : NULL
                                        // Returns HWND of desktop menu

   #define MDI_MSG_LOCATE_DOCUMENT_MENU   WM_USER+6
                                        // mp1 : NULL
                                        // mp2 : NULL
                                        // Returns HWND of document menu
                                        //  (currently active document if sent to desktop window)

   #define MDI_MSG_SET_DESKTOP_TITLE      WM_USER+7
                                        // mp1 : PCHAR szTitleText
                                        // mp2 : NULL
                                        // Sets the title text for the desktop. If this message is
                                        //  never sent, the title of the desktop as supplied in the
                                        //  MDICreateDesktop call is used.

   #define MDI_MSG_SET_INSTANCE_TITLE     WM_USER+8
                                        // mp1 : PCHAR szTitleText
                                        // mp2 : NULL
                                        // Sets the title text for the document instance. If this message is
                                        //  never sent, the title of the document as supplied in the
                                        //  MDICreateDocument call, suffixed if necessary with the instance index, is used.


// WM_CONTROL Notification codes
// -----------------------------
   #define MDI_NOTIFY_CREATE_DESKTOP    1
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_CREATE_DESKTOP)
                                        // mp2 : PVOID pvDeskData

   #define MDI_NOTIFY_DESTROY_DESKTOP   2
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_DESTROY_DESKTOP)
                                        // mp2 : PVOID pvDeskData

   #define MDI_NOTIFY_CREATE_DOCUMENT   3
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_CREATE_DOCUMENT)
                                        // mp2 : PVOID pvDocData

   #define MDI_NOTIFY_CREATE_INSTANCE   4
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_CREATE_INSTANCE)
                                        // mp2 : PVOID pvInstData

   #define MDI_NOTIFY_DESTROY_DOCUMENT  5
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_DESTROY_DOCUMENT)
                                        // mp2 : PVOID pvDocData

   #define MDI_NOTIFY_DESTROY_INSTANCE  6
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_DESTROY_INSTANCE)
                                        // mp2 : PVOID pvInstData

   #define MDI_INHIBIT_CLOSE_DESKTOP    7
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_INHIBIT_CLOSE_DESKTOP)
                                        // mp2 : PVOID pvDeskData
                                        // Return TRUE if the desktop close is to be inhibited;
                                        //  otherwise return NULL. A "request save" message may be issued
                                        //  to the user in response to this message.

   #define MDI_INHIBIT_CLOSE_INSTANCE   8
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_INHIBIT_CLOSE_INSTANCE)
                                        // mp2 : PVOID pvInstData
                                        // Return TRUE if the document instance close is to be inhibited;
                                        //  otherwise return NULL. A "request save" message may be issued
                                        //  to the user in response to this message.

   #define MDI_NOTIFY_CASCADE_INSTANCE  9
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_CASCADE_INSTANCE)
                                        // mp2 : PVOID pvInstData
                                        // Sent to each document instance that is NOT an icon
                                        // when a cascade operation is performed

   #define MDI_NOTIFY_TILE_INSTANCE     10
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_TILE_INSTANCE)
                                        // mp2 : PVOID pvInstData
                                        // Sent to each document instance that is NOT an icon
                                        // when a tile operation is performed

   #define MDI_NOTIFY_ACTIVATE_INSTANCE 11
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_ACTIVATE_INSTANCE)
                                        // mp2 : PVOID pvInstData

   #define MDI_NOTIFY_DEACTIVATE_INSTANCE 12
                                        // mp1 : MPFROM2SHORT (idDesktop, MDI_NOTIFY_DEACTIVATE_INSTANCE)
                                        // mp2 : PVOID pvInstData


// Function Declarations
// ---------------------
   HWND    EXPENTRY MDICreateDesktop    (PFNWP, ULONG, PCHAR, HMODULE, USHORT, PHWND, USHORT, USHORT, PCHAR, PCHAR, PCHAR, ULONG);
                                        // PFNWP   : your desktop window procedure
                                        // ULONG   : FCF_* flags for the desktop frame
                                        // PCHAR   : initial desktop title text
                                        // HMODULE : module handle for resources (or NULL if in .EXE)
                                        // USHORT  : id of resources
                                        // PHWND   : pointer to area in which handle to desktop client is returned
                                        // USHORT  : size of re-entrant desktop data
                                        // USHORT  : id of desktop frame
                                        // PCHAR   : text of menu entry before which document menu deltas are inserted
                                        // PCHAR   : text of menu entry before which the "window" menu entry is inserted
                                        // PCHAR   : desired text of "window" menu entry
                                        // ULONG   : mask of options (see below)

   #define MDI_KEEP_WINDOWS_MENU        0x00000001L // --don't remove the windows menu
   #define MDI_INITIAL_TILE_ALWAYS      0x00000002L // --set tile always as the initial state
   #define MDI_INITIAL_CASCADE_ALWAYS   0x00000004L // --set cascade always as the initial state
   #define MDI_INITIAL_MAX_DESKTOP      0x00000008L // --create the desktop as maximized
   #define MDI_KEEP_SCROLLBARS          0x00000010L // --don't remove scroll bars when the document loses the focus
   #define MDI_DISABLE_NEW_DOCUMENT     0x00000020L // --disables "new document" window menu entry
   #define MDI_DISABLE_DESKTOP_POSITION 0x00000040L // --disables memory of desktop position and size
   #define MDI_DISABLE_DOCUMENT_POSITION 0x0000080L // --disables memory of document position and size
   #define MDI_ALWAYS_ON_RESIZE         0x00000100L // --performs tile, cascade always on desktop resize

   HWND    EXPENTRY MDICreateDocument   (PFNWP, HWND, ULONG, PCHAR, HMODULE, USHORT, PHWND, USHORT, USHORT, USHORT);
                                        // PFNWP   : your document window procedure
                                        // HWND    : handle to the desktop client window, as returned by MDICreateDesktop
                                        // ULONG   : FCF_* flags for the document frame
                                        // PCHAR   : initial document title
                                        // HMODULE : module handle for resources (or NULL if in .EXE)
                                        // USHORT  : id of resources
                                        // PHWND   : pointer to area in which handle to document client is returned
                                        // USHORT  : size of re-entrant document data
                                        // USHORT  : size of re-entrant document instance data
                                        // USHORT  : id of document frame

   MRESULT EXPENTRY MDIDesktopWndProc   (HWND, USHORT, MPARAM, MPARAM);
                                        // HWND    : window handle
                                        // USHORT  : message id
                                        // MPARAM  : mp1
                                        // MPARAM  : mp2

   MRESULT EXPENTRY MDIDocumentWndProc  (HWND, USHORT, MPARAM, MPARAM);
                                        // HWND    : window handle
                                        // USHORT  : message id
                                        // MPARAM  : mp1
                                        // MPARAM  : mp2

   VOID    EXPENTRY MDIInitialize       (HAB);
                                        // HAB     : hab


// End of MDI.H
// ------------
   #endif
