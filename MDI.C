// Includes
// --------
   #define  INCL_WIN
   #define  INCL_DOS

   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <os2.h>

   #include "MDI.H"
   #include "MDI.RH"

   #include "AllocMem.H"
   #include "TellUser.H"


// Constants
// ---------
   #define MAX_STRING    256           // Maximum string size
   #define MAX_MENUDELTA  32           // Maximum menu delta ids
   #define INSERT_TITLE    1           // Insert title command code
   #define REMOVE_TITLE    2           // Remove title command code
   #define UPDATE_TITLE    3           // Update title command code
   #define HIDE_TITLE      4           // Hide or unhide title command code
   #define AWP_TILED       1           // Arrange tiled flag
   #define AWP_CASCADED    2           // Arrange cascaded flag
   #define CASC_EDGE_NUM   2           // Cascaded arrange edge
   #define CASC_EDGE_DENOM 3           // Cascaded arrange edge
   #define ICON_PARK_NUM   5           // Icon park
   #define ICON_PARK_DENOM 3           // Icon park

   const USHORT cbExtraData            // Extra window storage
              = (2 * sizeof (PVOID));


// Desktop Creation Parameters
// ---------------------------
   typedef struct _DESKTOPCREATEPARMS
     {USHORT cbDeskData;               // Size of user desktop data
      PCHAR pszDelta;                  // Menu item before which deltas go
      PCHAR pszExit;                   // Menu item before which "window" goes
      PCHAR pszWindow;                 // Window menu item text
     } DESKTOPCREATEPARMS;

   typedef DESKTOPCREATEPARMS *PDESKTOPCREATEPARMS;


// Document Creation Parameters
// ----------------------------
   typedef struct _DOCUMENTCREATEPARMS
     {USHORT cbDocData;                // Size of user document data
      USHORT cbInstData;               // Size of user instance data
      ULONG flCreateFlags;             // Copy of frame creation flags
      HMODULE hmodResources;           // Copy of frame resources
      USHORT idResources;              // Copy of frame resources id
      PFNWP pfnUser;                   // User window procedure
     } DOCUMENTCREATEPARMS;

   typedef DOCUMENTCREATEPARMS *PDOCUMENTCREATEPARMS;


// Desktop Intra-instance Data
// ---------------------------
   typedef struct _INSTDESKTOP
     {HWND hwndDocument;               // Active document window
      HWND hwndDesktop;                // Desktop window
      HWND hwndParent;                 // Its frame's parent
      HWND hwndFrame;                  // Its frame
      HWND hwndMenu;                   // Its menu
      HWND hwndMenuDelta;              // Its menu delta
      HACCEL haccel;                   // Its accelerator table
      CHAR szName[MAX_STRING];         // Its name
      CHAR szTitleText[MAX_STRING];    // Its titlebar text
      SWP swp;                         // Its position
      MENUITEM miSysMenu;              // Maximized child system menu
      PVOID pvDeskData;                // -->user desktop data
      BOOL fTileAlways;                // TRUE == Tile always
      BOOL fCascadeAlways;             // TRUE == Cascade always
      BOOL fAlwaysOnResize;            // TRUE == Tile, cascade always on resize
      BOOL fKeepWindowsMenu;           // TRUE == Keep windows menu
      BOOL fKeepScrollBars;            // TRUE == Keep scroll bars
      BOOL fDisableDesktopPosition;    // TRUE == Disable desktop position retention
      BOOL fDisableDocumentPosition;   // TRUE == Disable document position retention
      BOOL fDisableNewDocument;        // TRUE == Disable "new document" menu item
      PCHAR pszDelta;                  // Menu item before which deltas go
      MENUITEM miDelta;                // Delta menu
      PCHAR pszExit;                   // Menu item before which "window" goes
      PCHAR pszWindow;                 // Window menu item text
      MENUITEM miWindow;               // Window menu
      USHORT idDesktop;                // Desktop id
      USHORT cidDelta;                 // Count of menu delta ids
      USHORT idDelta[MAX_MENUDELTA];   // Menu delta ids
     } INSTDESKTOP;

   typedef INSTDESKTOP *PINSTDESKTOP;


// Document Intra-instance Data
// ----------------------------
   typedef struct _INSTDOCUMENT
     {HWND hwndDocument;               // Document window
      HWND hwndDesktop;                // Desktop window
      HWND hwndFrame;                  // Its frame
      HWND hwndMenu;                   // Its menu
      HWND hwndSysMenu;                // Its system menu
      HWND hwndTitleBar;               // Its title bar
      HWND hwndMinMax;                 // Its min/max buttons
      HWND hwndVScroll;                // Its vertical scroll bar
      HWND hwndHScroll;                // Its horizontal scroll bar
      USHORT idInternal;               // Its internal id
      HACCEL haccel;                   // Its accelerator table
      CHAR szName[MAX_STRING];         // Its name
      SHORT iName;                     // Its name index
      CHAR szTitleText[MAX_STRING];    // Its titlebar text
      SWP swp;                         // Its position
      USHORT usState;                  // Its state
      USHORT cbDocData;                // Size of user document data
      USHORT cbInstData;               // Size of user instance data
      PFNWP pfnUser;                   // User window procedure
      ULONG flCreateFlags;             // Frame creation flags
      HMODULE hmodResources;           // Frame resources
      USHORT idResources;              // Frame resources id
      USHORT idDesktop;                // Desktop id
      PVOID pvDocData;                 // -->user document data
      PVOID pvInstData;                // -->user instance data
     } INSTDOCUMENT;

   typedef INSTDOCUMENT *PINSTDOCUMENT;


// Function Declarations (see also MDI.H)
// --------------------------------------
   MRESULT       FAR      MDIDesktopClose            (PINSTDESKTOP);
   MRESULT       FAR      MDIDesktopCommand          (PINSTDESKTOP, USHORT);
   MRESULT       FAR      MDIDesktopCreate           (HWND, PDESKTOPCREATEPARMS);
   MRESULT       FAR      MDIDesktopDestroy          (PINSTDESKTOP);
   MRESULT       FAR      MDIDesktopMinMax           (PINSTDESKTOP, PSWP);
   MRESULT       FAR      MDIDesktopPaint            (PINSTDESKTOP);
   MRESULT       FAR      MDIDesktopSizeAndMove      (PINSTDESKTOP);
   MRESULT       FAR      MDIDocumentActivate        (PINSTDOCUMENT, BOOL);
   VOID          FAR      MDIDocumentArrange         (HWND, USHORT);
   VOID          FAR      MDIDocumentArrangeCascaded (PRECTL, SHORT, PSWP);
   VOID          FAR      MDIDocumentArrangeTiled    (PRECTL, SHORT, PSWP);
   VOID          FAR      MDIDocumentClone           (PINSTDOCUMENT);
   MRESULT       FAR      MDIDocumentClose           (PINSTDOCUMENT);
   MRESULT       FAR      MDIDocumentCommand         (PINSTDOCUMENT, USHORT);
   MRESULT       FAR      MDIDocumentCreate          (HWND, PDOCUMENTCREATEPARMS);
   MRESULT       FAR      MDIDocumentDestroy         (PINSTDOCUMENT);
   HWND          FAR      MDIDocumentFrameFromID     (HWND, USHORT);
   VOID          FAR      MDIDocumentHide            (PINSTDOCUMENT);
   MRESULT       FAR      MDIDocumentMinMax          (PINSTDOCUMENT, PSWP);
   MRESULT       EXPENTRY MDIDocumentMoreDlgProc     (HWND, USHORT, MPARAM, MPARAM);
   MRESULT       FAR      MDIDocumentPaint           (PINSTDOCUMENT);
   VOID          FAR      MDIDocumentShowFrameCtls   (PINSTDOCUMENT, BOOL, BOOL, BOOL);
   PINSTDOCUMENT FAR      MDIDocumentTitlesRebuild   (PINSTDOCUMENT, USHORT);
   MRESULT       EXPENTRY MDIDocumentUnhideDlgProc   (HWND, USHORT, MPARAM, MPARAM);
   HACCEL        FAR      MDILoadAccelTable          (USHORT);
   HWND          FAR      MDILoadDialog              (HWND, PFNWP, USHORT, PVOID);
   HWND          FAR      MDILoadMenu                (HWND, USHORT);
   VOID          FAR      MDILoadString              (USHORT, USHORT, PCHAR);


// Function: MDICreateDesktop - Create MDI Desktop
// -----------------------------------------------
   HWND EXPENTRY MDICreateDesktop (pfnUser, flFrameFlags, pszName, hmodResources, idResources,
     phwndClient, cbDeskData, idDesktop, pszDelta, pszExit, pszWindow, flOptions)

      PFNWP pfnUser;                   // User window procedure
      ULONG flFrameFlags;              // Frame creation flags
      PCHAR pszName;                   // Desktop name
      HMODULE hmodResources;           // Module that contains resources
      USHORT idResources;              // Resources identifier
      PHWND phwndClient;               // -->area to receive desktop handle
      USHORT cbDeskData;               // Size of user desktop data
      USHORT idDesktop;                // ID of desktop
      PCHAR pszDelta;                  // Text of menu entry before which menu deltas are inserted
      PCHAR pszExit;                   // Text of menu entry before which the "window" menu is inserted
      PCHAR pszWindow;                 // Desired "Window" menu text
      ULONG flOptions;                 // Options

   // Define function data

     {HWND hwndFrame;                  // Frame window
      FRAMECDATA fcdata;               // Frame creation parameters
      DESKTOPCREATEPARMS deskcp;       // Desktop creation parameters
      PINSTDESKTOP pinst;              // -->desktop instance data

   // Initialize frame creation parameters

      fcdata.cb = sizeof (fcdata);
      fcdata.flCreateFlags = flFrameFlags;
      fcdata.hmodResources = hmodResources;
      fcdata.idResources = idResources;

   // Initialize desktop creation parameters

      deskcp.cbDeskData = cbDeskData;
      deskcp.pszDelta = pszDelta;
      deskcp.pszExit = pszExit;
      deskcp.pszWindow = pszWindow;

   // Create desktop frame and client windows

      hwndFrame = WinCreateWindow (HWND_DESKTOP, WC_FRAME, pszName,
        WS_VISIBLE, 0, 0, 0, 0, (PVOID) NULL, HWND_TOP, idDesktop, &fcdata, (PVOID) NULL);
      if (hwndFrame == (PVOID) NULL) return (HWND) NULL;
      *phwndClient = WinCreateWindow (hwndFrame, MDI_DESKTOPCLASS, (PSZ) NULL,
        WS_VISIBLE, 0, 0, 0, 0, hwndFrame, HWND_BOTTOM, FID_CLIENT, &deskcp, (PVOID) NULL);
      if (*phwndClient == (HWND) NULL) return (HWND) NULL;

   // Subclass desktop window procedure

      if (pfnUser != (PFNWP) NULL)
         WinSubclassWindow (*phwndClient, pfnUser);

   // Locate desktop instance data

      pinst = (PINSTDESKTOP) WinQueryWindowULong (*phwndClient, QWL_USER + sizeof (PVOID));

   // Tell user that desktop has been created

      WinSendMsg (*phwndClient, WM_CONTROL,
        MPFROM2SHORT (pinst->idDesktop, MDI_NOTIFY_CREATE_DESKTOP),
        (MPARAM) pinst->pvDeskData);

   // Handle option flags

      // Handle "keep windows menu"

         if (flOptions & MDI_KEEP_WINDOWS_MENU)
           {pinst->fKeepWindowsMenu = TRUE;
            WinSendMsg (pinst->hwndMenu, MM_INSERTITEM,
              (MPARAM) &(pinst->miWindow), (MPARAM) pinst->pszWindow);
           }

      // Handle "keep scroll bars"

         if (flOptions & MDI_KEEP_SCROLLBARS)
            pinst->fKeepScrollBars = TRUE;

      // Handle "disable new document"

         if (flOptions & MDI_DISABLE_NEW_DOCUMENT)
           {pinst->fDisableNewDocument = TRUE;
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_DELETEITEM,
              MPFROM2SHORT (CMD_NEW_DOCUMENT, TRUE), (MPARAM) NULL);
           }

      // Handle "disable desktop position"

         if (flOptions & MDI_DISABLE_DESKTOP_POSITION)
            pinst->fDisableDesktopPosition = TRUE;

      // Handle "disable document position"

         if (flOptions & MDI_DISABLE_DOCUMENT_POSITION)
            pinst->fDisableDocumentPosition = TRUE;

      // Handle "always on resize"

         if (flOptions & MDI_ALWAYS_ON_RESIZE)
            pinst->fAlwaysOnResize = TRUE;

      // Handle "initial tile always"

         if (flOptions & MDI_INITIAL_TILE_ALWAYS)
           {pinst->fTileAlways = TRUE;
            pinst->fCascadeAlways = FALSE;
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_SETITEMATTR,
              MPFROM2SHORT (CMD_TILE_ALWAYS, FALSE),
              MPFROM2SHORT (MIA_CHECKED, MIA_CHECKED));
           }

      // Handle "initial cascade always"

         if (flOptions & MDI_INITIAL_CASCADE_ALWAYS)
           {pinst->fTileAlways = FALSE;
            pinst->fCascadeAlways = TRUE;
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_SETITEMATTR,
              MPFROM2SHORT (CMD_CASCADE_ALWAYS, FALSE),
              MPFROM2SHORT (MIA_CHECKED, MIA_CHECKED));
           }

      // Handle "initial maximized desktop"

         if (flOptions & MDI_INITIAL_MAX_DESKTOP)
            WinSendMsg (hwndFrame, WM_SYSCOMMAND,
              MPFROMSHORT ((CMD_DESKTOP_MAXIMIZE - CMD_DESKTOP_BASE) | SC_SIZE),
              MPFROM2SHORT (CMDSRC_ACCELERATOR, FALSE));

   // Return

      return hwndFrame;

     }


// Function: MDICreateDocument - Create MDI Document
// -------------------------------------------------
   HWND EXPENTRY MDICreateDocument (pfnUser, hwndParent, flFrameFlags, pszName,
                               hmodResources, idResources, phwndClient, cbDocData, cbInstData, idDocument)

      PFNWP pfnUser;                   // User window procedure
      HWND hwndParent;                 // Document parent
      ULONG flFrameFlags;              // Frame creation flags
      PCHAR pszName;                   // Document name
      HMODULE hmodResources;           // Module that contains resources
      USHORT idResources;              // Resources identifier
      PHWND phwndClient;               // -->area to receive document handle
      USHORT cbDocData;                // Size of user document data
      USHORT cbInstData;               // Size of user document data
      USHORT idDocument;               // Document frame id

   // Define function data

     {HWND hwndFrame;                  // Frame window
      FRAMECDATA fcdata;               // Frame creation parameters
      DOCUMENTCREATEPARMS doccp;       // Document creation parameters
      PINSTDOCUMENT pinst;             // -->document instance data

   // Initialize frame creation parameters

      fcdata.cb = sizeof (fcdata);
      fcdata.flCreateFlags = flFrameFlags;
      fcdata.hmodResources = hmodResources;
      fcdata.idResources = idResources;

   // Initialize document creation parameters

      doccp.cbDocData = cbDocData;
      doccp.cbInstData = cbInstData;
      doccp.pfnUser = pfnUser;
      doccp.flCreateFlags = flFrameFlags;
      doccp.hmodResources = hmodResources;
      doccp.idResources = idResources;

   // Create document frame and client windows

      hwndFrame = WinCreateWindow (hwndParent, WC_FRAME, pszName,
        WS_VISIBLE, 0, 0, 0, 0, (PVOID) NULL, HWND_TOP, idDocument, &fcdata, (PVOID) NULL);
      if (hwndFrame == (HWND) NULL) return (HWND) NULL;
      *phwndClient = WinCreateWindow (hwndFrame, MDI_DOCUMENTCLASS, (PSZ) NULL,
        WS_VISIBLE, 0, 0, 0, 0, hwndFrame, HWND_BOTTOM, FID_CLIENT, &doccp, (PVOID) NULL);
      if (*phwndClient == (HWND) NULL) return (HWND) NULL;

   // Subclass document window procedure

      if (pfnUser != (PFNWP) NULL)
         WinSubclassWindow (*phwndClient, pfnUser);

   // Tell user that document and instance has been created

      pinst = (PINSTDOCUMENT) WinQueryWindowULong (*phwndClient, QWL_USER + sizeof (PVOID));
      if (pinst->iName <= 1)
         WinSendMsg (*phwndClient, WM_CONTROL,
           MPFROM2SHORT (pinst->idDesktop, MDI_NOTIFY_CREATE_DOCUMENT),
           (MPARAM) pinst->pvDocData);
      WinSendMsg (*phwndClient, WM_CONTROL,
        MPFROM2SHORT (pinst->idDesktop, MDI_NOTIFY_CREATE_INSTANCE),
        (MPARAM) pinst->pvInstData);

   // Return

      return hwndFrame;

     }


// Function: MDIDesktopClose - Close MDI Desktop
// ---------------------------------------------
   MRESULT FAR MDIDesktopClose (pinst)

      PINSTDESKTOP pinst;              // -->desktop instance data

   // Define function data

     {PINSTDOCUMENT pinstDocument;     // -->document instance data
      HWND hwnd, hwndNext;             // Document window
      BOOL fNoQuit = FALSE;            // TRUE == don't quit

   // Ask if the desktop can be closed; if it can't nothing else changes

      if ((BOOL) SHORT1FROMMR (WinSendMsg (pinst->hwndDesktop, WM_CONTROL,
       MPFROM2SHORT (pinst->idDesktop, MDI_INHIBIT_CLOSE_DESKTOP), (MPARAM) pinst->pvDeskData)) == TRUE)
         return (MRESULT) NULL;

   // Close all open documents if the user lets us

      for (hwnd = WinQueryWindow (pinst->hwndDesktop, QW_TOP, FALSE);
       hwnd; hwnd = WinQueryWindow (hwnd, QW_NEXT, FALSE))
        {retryClose: if ((WinQueryWindow (hwnd, QW_OWNER, FALSE) == (HWND) NULL)
          && (WinQueryWindowUShort (hwnd, QWS_ID) != 0))
           {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong
             (WinWindowFromID (hwnd, FID_CLIENT), QWL_USER + sizeof (PVOID));
            if ((BOOL) SHORT1FROMMR (WinSendMsg (pinstDocument->hwndDocument, WM_CONTROL,
             MPFROM2SHORT (pinstDocument->idDesktop, MDI_INHIBIT_CLOSE_DESKTOP), (MPARAM) pinstDocument->pvInstData)) == FALSE)
              {hwndNext = WinQueryWindow (hwnd, QW_NEXT, FALSE);
               WinDestroyWindow (hwnd);
               if ((hwnd = hwndNext) == NULL) break;
               goto retryClose;
              }
            else fNoQuit = TRUE;
           }
        }

   // Quit if the user lets us

      if (!fNoQuit)
        {WinDestroyWindow (pinst->hwndFrame);
         WinPostMsg ((HWND) NULL, WM_QUIT, NULL, NULL);
        }

   // Indicate close handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopCommand - Handle MDI Desktop Command
// --------------------------------------------------------
   MRESULT FAR MDIDesktopCommand (pinst, usCommand)

      PINSTDESKTOP pinst;              // -->desktop instance data
      USHORT usCommand;                // WM_COMMAND code

   // Define function data

     {PINSTDOCUMENT pinstDocument;     // -->document instance data
      HWND hdlg;                       // Unhide dialog box

   // Act upon command code

      switch (usCommand)

   // Handle Desktop system menu command by passing the system equivalent
   // to the desktop frame

        {case CMD_DESKTOP_SIZE:
         case CMD_DESKTOP_MOVE:
         case CMD_DESKTOP_MINIMIZE:
         case CMD_DESKTOP_MAXIMIZE:
         case CMD_DESKTOP_CLOSE:
         case CMD_DESKTOP_NEXT:
         case CMD_DESKTOP_APPMENU:
         case CMD_DESKTOP_SYSMENU:
         case CMD_DESKTOP_RESTORE:
            WinSendMsg (pinst->hwndFrame, WM_SYSCOMMAND,
              MPFROMSHORT ((usCommand - CMD_DESKTOP_BASE) | SC_SIZE),
              MPFROM2SHORT (CMDSRC_ACCELERATOR, FALSE));
            break;

   // Handle Maximized Document system menu command by applying
   // the command to the active document

         case CMD_DOCUMENT_SIZE:
         case CMD_DOCUMENT_MOVE:
         case CMD_DOCUMENT_MINIMIZE:
         case CMD_DOCUMENT_MAXIMIZE:
         case CMD_DOCUMENT_CLOSE:
         case CMD_DOCUMENT_NEXT:
         case CMD_DOCUMENT_APPMENU:
         case CMD_DOCUMENT_SYSMENU:
         case CMD_DOCUMENT_RESTORE:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument,
                 QWL_USER + sizeof (PVOID));
               if (pinstDocument->usState == SWP_MAXIMIZE)
                 {if (usCommand == CMD_DOCUMENT_RESTORE)
                    {WinSetParent (pinstDocument->hwndMinMax, pinstDocument->hwndFrame, FALSE);
                     WinSetWindowPos (pinstDocument->hwndFrame, (HWND) NULL, 0, 0, 0, 0, SWP_RESTORE);
                    }
                  else if (usCommand == CMD_DOCUMENT_NEXT)
                     WinSendMsg (pinstDocument->hwndFrame, WM_SYSCOMMAND,
                       MPFROMSHORT (SC_NEXT), MPFROM2SHORT (CMDSRC_MENU, FALSE));
                  else if (usCommand == CMD_DOCUMENT_CLOSE)
                     WinDestroyWindow (pinstDocument->hwndFrame);
                 }
              }
            else WinAlarm (HWND_DESKTOP, WA_WARNING);
            break;

   // Handle arrange tiled request

         case CMD_ARRANGE_TILED:
            MDIDocumentArrange (pinst->hwndDesktop, AWP_TILED);
            break;

   // Handle arrange cascaded request

         case CMD_ARRANGE_CASCADED:
            MDIDocumentArrange (pinst->hwndDesktop, AWP_CASCADED);
            break;

   // Handle tile always request by reseting flags and checking menu items;
   // then we perform actual tile

         case CMD_TILE_ALWAYS:
            pinst->fTileAlways = ! pinst->fTileAlways;
            pinst->fCascadeAlways = FALSE;
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_SETITEMATTR,
              MPFROM2SHORT (CMD_TILE_ALWAYS, FALSE),
              MPFROM2SHORT (MIA_CHECKED, (pinst->fTileAlways)? MIA_CHECKED : FALSE));
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_SETITEMATTR,
              MPFROM2SHORT (CMD_CASCADE_ALWAYS, FALSE), MPFROM2SHORT (MIA_CHECKED, FALSE));
            if (pinst->fTileAlways)
               MDIDocumentArrange (pinst->hwndDesktop, AWP_TILED);
            break;

   // Handle cascade always request by reseting flags and checking menu items;
   // then we perform actual cascade

         case CMD_CASCADE_ALWAYS:
            pinst->fCascadeAlways = ! pinst->fCascadeAlways;
            pinst->fTileAlways = FALSE;
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_SETITEMATTR,
              MPFROM2SHORT (CMD_CASCADE_ALWAYS, FALSE),
              MPFROM2SHORT (MIA_CHECKED, (pinst->fCascadeAlways)? MIA_CHECKED : FALSE));
            WinSendMsg (pinst->miWindow.hwndSubMenu, MM_SETITEMATTR,
              MPFROM2SHORT (CMD_TILE_ALWAYS, FALSE), MPFROM2SHORT (MIA_CHECKED, FALSE));
            if (pinst->fCascadeAlways)
               MDIDocumentArrange (pinst->hwndDesktop, AWP_CASCADED);
            break;

   // Handle request to hide document by removing its entry from the
   // window submenu, activating the next entry in the submenu (if any)
   // and hiding the document

         case CMD_HIDE:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument,
                 QWL_USER + sizeof (PVOID));
               MDIDocumentHide (pinstDocument);
              }
            break;

   // Handle request to unhide document using the unhide dialog procedure

         case CMD_UNHIDE:
            hdlg = MDILoadDialog (pinst->hwndDesktop, MDIDocumentUnhideDlgProc,
               DIALOG_UNHIDE, (PVOID) pinst);
            WinProcessDlg (hdlg);
            WinDestroyWindow (hdlg);
            break;

   // Handle request for a new document by creating the document
   // based on the currently active document

         case CMD_NEW_DOCUMENT:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument,
                 QWL_USER + sizeof (PVOID));
               MDIDocumentClone (pinstDocument);
              }
            break;

   // Handle request to display the "More Documents" dialog

         case CMD_MORE_DOCUMENTS:
            hdlg = MDILoadDialog (pinst->hwndDesktop, MDIDocumentMoreDlgProc,
               DIALOG_MORE, (PVOID) pinst);
            WinProcessDlg (hdlg);
            WinDestroyWindow (hdlg);
            break;

   // Handle request to activate document

         default:
            if (usCommand >= CMD_SELECT_DOCUMENT)
               WinSetWindowPos (MDIDocumentFrameFromID (pinst->hwndDesktop, usCommand),
                 HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);
            else if ((usCommand < CMD_DESKTOP_BASE) && (pinst->hwndDocument != (HWND) NULL))
               WinSendMsg (pinst->hwndDocument, WM_COMMAND,
                 MPFROMSHORT (usCommand), MPFROM2SHORT (CMDSRC_ACCELERATOR, FALSE));
            break;

   // Complete handling of command code

        }

   // Indicate command handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopCreate - Create MDI Desktop
// -----------------------------------------------
   MRESULT FAR MDIDesktopCreate (hwndDesktop, pdeskcp)

      HWND hwndDesktop;                // Desktop window
      PDESKTOPCREATEPARMS pdeskcp;     // -->desktop creation parameters

   // Define function data

     {PINSTDESKTOP pinst;              // -->instance data
      BOOL fIniDatFound;               // TRUE == OS2.INI data found
      USHORT cbBuf;                    // Length of OS2.INI data
      SHORT xLeft, xRight;             // X indentation factors
      SHORT yBottom, yTop;             // Y indentation factors
      CHAR szText[MAX_STRING];         // Menu item text

   // Acquire instance data and save in MDI desktop window

      pinst = AllocMemAlloc ((LONG) (sizeof (*pinst)));
      WinSetWindowULong (hwndDesktop, QWL_USER + sizeof (PVOID), (ULONG) pinst);

   // Copy desktop creation parameters if available

      if (pdeskcp != (PDESKTOPCREATEPARMS) NULL)
        {pinst->pszDelta = pdeskcp->pszDelta;
         pinst->pszExit = pdeskcp->pszExit;
         pinst->pszWindow = pdeskcp->pszWindow;
        }

   // Initialize inter-window relationships

      pinst->hwndDesktop = hwndDesktop;
      pinst->hwndFrame = WinQueryWindow (pinst->hwndDesktop, QW_PARENT, FALSE);
      pinst->hwndParent = WinQueryWindow (pinst->hwndFrame, QW_PARENT, FALSE);
      pinst->hwndMenu = WinWindowFromID (pinst->hwndFrame, FID_MENU);
      pinst->idDesktop = WinQueryWindowUShort (pinst->hwndFrame, QWS_ID);

   // Verify inter-window relationships

      while (pinst->hwndMenu == (HWND) NULL)
         TellUser (ERROR_DESKTOP_NO_MENU, MDI_MODNAME, MB_ABORTRETRYIGNORE | MB_ICONHAND);

   // Find location in desktop menu at which to insert the document menu deltas

      pinst->miDelta.iPosition = 0;
      while ((pinst->miDelta.id = SHORT1FROMMR (WinSendMsg (pinst->hwndMenu, MM_ITEMIDFROMPOSITION,
       MPFROMSHORT (pinst->miDelta.iPosition), (MPARAM) NULL))) != MID_ERROR)
        {WinSendMsg (pinst->hwndMenu, MM_QUERYITEMTEXT, MPFROM2SHORT (pinst->miDelta.id,
           sizeof (szText)), (MPARAM) szText);
         if (strcmpi (szText, pinst->pszDelta) == 0) break;
         pinst->miDelta.iPosition++;
        }

   // Verify that a location for the window sub-menu was found

      while (pinst->miDelta.id == MID_ERROR)
         TellUser (ERROR_DESKTOP_NO_DELTA, MDI_MODNAME, MB_ABORTRETRYIGNORE | MB_ICONHAND, pinst->pszDelta);

   // Find location in desktop menu at which to insert the window sub-menu

      pinst->miWindow.iPosition = 0;
      while ((pinst->miWindow.id = SHORT1FROMMR (WinSendMsg (pinst->hwndMenu, MM_ITEMIDFROMPOSITION,
       MPFROMSHORT (pinst->miWindow.iPosition), (MPARAM) NULL))) != MID_ERROR)
        {WinSendMsg (pinst->hwndMenu, MM_QUERYITEMTEXT, MPFROM2SHORT (pinst->miWindow.id,
           sizeof (szText)), (MPARAM) szText);
         if (strcmpi (szText, pinst->pszExit) == 0) break;
         pinst->miWindow.iPosition++;
        }

   // Verify that a location for the window sub-menu was found

      while (pinst->miWindow.id == MID_ERROR)
         TellUser (ERROR_DESKTOP_NO_EXIT, MDI_MODNAME, MB_ABORTRETRYIGNORE | MB_ICONHAND, pinst->pszExit);

   // Initialize the window menu

      pinst->miWindow.afStyle = MIS_TEXT | MIS_SUBMENU;
      pinst->miWindow.afAttribute = 0;
      pinst->miWindow.id = MENU_WINDOW;
      pinst->miWindow.hwndSubMenu = MDILoadMenu (pinst->hwndMenu, MENU_WINDOW);
      pinst->miWindow.hItem = (ULONG) NULL;

   // Initialize the maximized child system menu

      pinst->miSysMenu.iPosition = 0;
      pinst->miSysMenu.afStyle = MIS_BITMAP | MIS_SUBMENU;
      pinst->miSysMenu.afAttribute = 0;
      pinst->miSysMenu.id = MENU_MAXCHILD_SYSMENU;
      pinst->miSysMenu.hwndSubMenu = MDILoadMenu (pinst->hwndMenu, MENU_MAXCHILD_SYSMENU);
      pinst->miSysMenu.hItem = (ULONG) WinGetSysBitmap (HWND_DESKTOP, SBMP_CHILDSYSMENU);

   // Load accelerator table

      pinst->haccel = MDILoadAccelTable (ACCEL_DESKTOP);
      WinSetAccelTable ((HAB) NULL, pinst->haccel, pinst->hwndFrame);

   // Determine initial name

      WinQueryWindowText (pinst->hwndFrame, sizeof (pinst->szName), pinst->szName);

   // Create user desktop data if applicable

      if (pdeskcp && (pdeskcp->cbDeskData > 0))
         pinst->pvDeskData = AllocMemAlloc ((LONG) (pdeskcp->cbDeskData));

   // Recall window position from OS2.INI file, if available

      cbBuf = sizeof (pinst->swp);
      pinst->swp.fs = 0;
      if (! pinst->fDisableDesktopPosition)
        fIniDatFound = WinQueryProfileData ((HAB) NULL, MDI_DESKTOPCLASS,
          pinst->szName, &(pinst->swp), &cbBuf);
      else fIniDatFound = FALSE;

   // If no prior position was stored, or if the window was maximized,
   // minimized or hidden, find a default position

      if (! fIniDatFound || (pinst->swp.fs & (SWP_MINIMIZE | SWP_MAXIMIZE /* | SWP_HIDE */)))
        {xLeft = (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);
         xRight = (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);
         yTop = (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYSIZEBORDER);
         yBottom = 2 * (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYICON);
         pinst->swp.x = xLeft;
         pinst->swp.y = yBottom;
         pinst->swp.cx = (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN) - xLeft - xRight;
         pinst->swp.cy = (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN) - yBottom - yTop;
         pinst->swp.fs &= ~SWP_HIDE;
        }

   // Display and position the desktop window

      pinst->swp.fs |= SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ZORDER;
      pinst->swp.hwndInsertBehind = HWND_TOP;
      pinst->swp.hwnd = pinst->hwndFrame;
      WinSetMultWindowPos ((HAB) NULL, &(pinst->swp), 1);

   // Indicate create handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopDestroy - Destroy MDI Desktop
// -------------------------------------------------
   MRESULT FAR MDIDesktopDestroy (pinst)

      PINSTDESKTOP pinst;              // -->desktop instance data

   // Save window position onto OS2.INI file

     {pinst->swp.fs &= ~SWP_HIDE;
      if (! pinst->fDisableDesktopPosition)
        WinWriteProfileData ((HAB) NULL, MDI_DESKTOPCLASS, pinst->szName,
          &(pinst->swp), sizeof (pinst->swp));

   // Free accelerator table

      WinDestroyAccelTable (pinst->haccel);

   // Tell user desktop will be destroyed

      WinSendMsg (pinst->hwndDesktop, WM_CONTROL,
        MPFROM2SHORT (pinst->idDesktop, MDI_NOTIFY_DESTROY_DESKTOP),
        (MPARAM) pinst->pvDeskData);

   // Free user desktop data if applicable

      if (pinst->pvDeskData)
         AllocMemFree (pinst->pvDeskData);

   // Free instance data

      AllocMemFree (pinst);

   // Indicate destroy handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopMinMax - Minimize/Maximize MDI Desktop
// ----------------------------------------------------------
   MRESULT FAR MDIDesktopMinMax (pinst, pswp)

      PINSTDESKTOP pinst;              // -->desktop instance data
      PSWP pswp;                       // -->desktop position information

   // Define function data

     {PINSTDOCUMENT pinstDocument;     // -->document instance data
      HWND hwndDocument;               // Document window
      HWND hwnd;                       // Document window handle
      ULONG ulStyle;                   // Document window style

   // Fool the compiler

      pswp = pswp;

   // We restore all maximized documents

      for (hwnd = WinQueryWindow (pinst->hwndDesktop, QW_TOP, FALSE);
       hwnd; hwnd = WinQueryWindow (hwnd, QW_NEXT, FALSE))

         // Determine document style

        {ulStyle = WinQueryWindowULong (hwnd, QWL_STYLE);

         // Ignore documents that are invisible or that do not
         // belong to the desktop

         if (WinQueryWindow (hwnd, QW_OWNER, FALSE)
          || (WinQueryWindowUShort (hwnd, QWS_ID) == 0)
          || !(ulStyle & WS_VISIBLE)) continue;

         // If the document is maximized, restore it

         if (ulStyle & WS_MAXIMIZED)
           {hwndDocument = WinWindowFromID (hwnd, FID_CLIENT);
            pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong
              (hwndDocument, QWL_USER + sizeof (PVOID));
            WinSetParent (pinstDocument->hwndMinMax, pinstDocument->hwndFrame, FALSE);
            WinSetWindowPos (hwnd, (HWND) NULL, 0, 0, 0, 0, SWP_RESTORE);
            if (pinst->hwndDocument != hwndDocument)
               MDIDocumentShowFrameCtls (pinstDocument, FALSE, FALSE, TRUE);
           }

         // Loop until all documents processed

        }

   // Indicate min/max handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopPaint - Paint MDI Desktop
// ---------------------------------------------
   MRESULT FAR MDIDesktopPaint (pinst)

      PINSTDESKTOP pinst;              // -->desktop instance data

   // Define function data

     {HPS hps;                         // Presentation space
      RECTL rcl;                       // Update rectangle

   // Paint a SYSCLR_WINDOW background

      hps = WinBeginPaint (pinst->hwndDesktop, (HPS) NULL, &rcl);
      WinFillRect (hps, &rcl, SYSCLR_APPWORKSPACE);
      WinEndPaint (hps);

   // Indicate paint handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopSizeAndMove - Size and Move Documents in MDI Desktop
// ---------------------------------------------------------------------
   MRESULT FAR MDIDesktopSizeAndMove (pinst)

      PINSTDESKTOP pinst;              // -->desktop instance data

   // Define function data

     {PINSTDOCUMENT pinstDocument;     // -->document instance data
      HWND hwnd;                       // Document window handle
      ULONG ulStyle;                   // Window style
      SWP swpDesktop;                  // Current desktop position
      SWP swpDocument;                 // Current document position

   // Capture new desktop position, saving the current position

      swpDesktop = pinst->swp;
      WinQueryWindowPos (pinst->hwndFrame, &(pinst->swp));

   // Don't do anything if the size hasn't changed

      if ((swpDesktop.cx == pinst->swp.cx) && (swpDesktop.cy == pinst->swp.cy))
         return (MRESULT) NULL;

   // Don't do anything if the desktop is (or was) minimized

      ulStyle = WinQueryWindowULong (pinst->hwndFrame, QWL_STYLE);
      if ((ulStyle & WS_MINIMIZED) || (swpDesktop.fs & SWP_MINIMIZE))
         return (MRESULT) NULL;

   // If tile, cascade always on resize is in effect, perform operation

      if (pinst->fAlwaysOnResize && (pinst->fCascadeAlways || pinst->fTileAlways))
        {if (pinst->fTileAlways)
            WinPostMsg (pinst->hwndDesktop, WM_COMMAND, MPFROMSHORT (CMD_ARRANGE_TILED), (MPARAM) NULL);
         else if (pinst->fCascadeAlways)
            WinPostMsg (pinst->hwndDesktop, WM_COMMAND, MPFROMSHORT (CMD_ARRANGE_CASCADED), (MPARAM) NULL);
        }

   // Otherwise, we move all documents that are themselves not minimized

      else
        {for (hwnd = WinQueryWindow (pinst->hwndDesktop, QW_TOP, FALSE);
          hwnd; hwnd = WinQueryWindow (hwnd, QW_NEXT, FALSE))

            // Determine document style

           {ulStyle = WinQueryWindowULong (hwnd, QWL_STYLE);

            // Ignore documents that are invisible or that do not
            // belong to the desktop

            if (WinQueryWindow (hwnd, QW_OWNER, FALSE)
             || (WinQueryWindowUShort (hwnd, QWS_ID) == 0)
             || !(ulStyle & WS_VISIBLE)) continue;

            // If the document is not minimized move relative to its top left corner

            if (!(ulStyle & WS_MINIMIZED))
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong
                 (WinWindowFromID (hwnd, FID_CLIENT), QWL_USER + sizeof (PVOID));
               WinQueryWindowPos (pinstDocument->hwndFrame, &swpDocument);
               WinSetWindowPos (pinstDocument->hwndFrame, (HWND) NULL,
                 swpDocument.x, swpDocument.y + (swpDesktop.y - pinst->swp.y), 0, 0, SWP_MOVE);
              }

            // Loop until all documents processed

           }

        }

   // Indicate size/move handled

      return (MRESULT) NULL;

     }


// Function: MDIDesktopWndProc - MDI Desktop window procedure
// ----------------------------------------------------------
   MRESULT EXPENTRY MDIDesktopWndProc (hwndDesktop, msg, mp1, mp2)

      HWND hwndDesktop;                // Desktop window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {PINSTDESKTOP pinst;              // -->instance data
      PINSTDOCUMENT pinstDocument;     // -->document instance data
      CHAR szName[MAX_STRING];         // Title text

   // Locate instance data

      pinst = (PINSTDESKTOP) WinQueryWindowULong (hwndDesktop, QWL_USER + sizeof (PVOID));

   // Analyze and process message

      switch (msg)

        {case WM_CLOSE:
            return MDIDesktopClose (pinst);

         case WM_COMMAND:
            return MDIDesktopCommand (pinst, LOUSHORT (mp1));

         case WM_CREATE:
            return MDIDesktopCreate (hwndDesktop, (PDESKTOPCREATEPARMS) mp1);

         case WM_DESTROY:
            return MDIDesktopDestroy (pinst);

         case WM_INITMENU:
            if (pinst->hwndDocument != (HWND) NULL)
               WinSendMsg (pinst->hwndDocument, msg, mp1, mp2);
            return (MRESULT) NULL;

         case WM_MENUEND:
            if (pinst->hwndDocument != (HWND) NULL)
               WinSendMsg (pinst->hwndDocument, msg, mp1, mp2);
            return (MRESULT) NULL;

         case WM_MINMAXFRAME:
            return MDIDesktopMinMax (pinst, (PSWP) mp1);

         case WM_MOVE:
            return MDIDesktopSizeAndMove (pinst);

         case WM_PAINT:
            return MDIDesktopPaint (pinst);

         case WM_SIZE:
            return MDIDesktopSizeAndMove (pinst);

         case MDI_MSG_LOCATE_DESKTOP_DATA:
            return (MRESULT) pinst->pvDeskData;

         case MDI_MSG_LOCATE_DOCUMENT_DATA:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument, QWL_USER + sizeof (PVOID));
               return (MRESULT) pinstDocument->pvDocData;
              }
            else return (MRESULT) NULL;

         case MDI_MSG_LOCATE_INSTANCE_DATA:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument, QWL_USER + sizeof (PVOID));
               return (MRESULT) pinstDocument->pvInstData;
              }
            else return (MRESULT) NULL;

         case MDI_MSG_LOCATE_ACTIVE_DOCUMENT:
            return (MRESULT) pinst->hwndDocument;

         case MDI_MSG_LOCATE_DESKTOP_MENU:
            return (MRESULT) pinst->hwndMenu;

         case MDI_MSG_LOCATE_DOCUMENT_MENU:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument, QWL_USER + sizeof (PVOID));
               return (MRESULT) pinstDocument->hwndMenu;
              }
            else return (MRESULT) NULL;

         case MDI_MSG_SET_DESKTOP_TITLE:
            strcpy (pinst->szTitleText, (PCHAR) mp1);
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument, QWL_USER + sizeof (PVOID));
               if (pinstDocument->usState == SWP_MAXIMIZE)
                 {if ((pinstDocument->iName == 0) || (pinstDocument->szTitleText[0]))
                     sprintf (szName, "%s - %s", (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName,
                      (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
                  else sprintf (szName, "%s - %s:%d", (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName,
                      (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName, pinstDocument->iName);
                  WinSetWindowText (pinst->hwndFrame, szName);
                 }
               else WinSetWindowText (pinst->hwndFrame, pinst->szTitleText);
              }
            else WinSetWindowText (pinst->hwndFrame, pinst->szTitleText);
            return (MRESULT) NULL;

         case MDI_MSG_SET_INSTANCE_TITLE:
            if (pinst->hwndDocument != (HWND) NULL)
              {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (pinst->hwndDocument, QWL_USER + sizeof (PVOID));
               strcpy (pinstDocument->szTitleText, (PCHAR) mp1);
               MDIDocumentTitlesRebuild (pinstDocument, UPDATE_TITLE);
              }
            WinSendMsg (pinst->hwndDesktop, MDI_MSG_SET_DESKTOP_TITLE,
              (MPARAM) (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName, (MPARAM) NULL);
            return (MRESULT) NULL;

        }

   // Return to PM

      return WinDefWindowProc (hwndDesktop, msg, mp1, mp2);

     }


// Function: MDIDocumentActivate - Activate MDI Document
// -----------------------------------------------------
   MRESULT FAR MDIDocumentActivate (pinst, fActivate)

      PINSTDOCUMENT pinst;             // -->document instance data
      BOOL fActivate;                  // TRUE == document is being activated

   // Define function data

     {PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      CHAR szName[MAX_STRING];         // Desktop name
      MENUITEM mi;                     // Menu item
      USHORT iItem;                    // Menu item position
      USHORT usNotify;                 // Notification code

   // Locate the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // Notify document instance of activation/deactivation

      usNotify = (fActivate)? MDI_NOTIFY_ACTIVATE_INSTANCE : MDI_NOTIFY_DEACTIVATE_INSTANCE;
      WinSendMsg (pinst->hwndDocument, WM_CONTROL,
        MPFROM2SHORT (pinst->idDesktop, usNotify), (MPARAM) pinst->pvInstData);

   // Show/hide all frame controls if document is not maximized or minimized

      if ((pinst->usState != SWP_MAXIMIZE) && (pinst->usState != SWP_MINIMIZE))
         MDIDocumentShowFrameCtls (pinst, fActivate, FALSE, TRUE);

   // Set the handle of the active document window in the desktop

      pinstDesktop->hwndDocument = (fActivate)? pinst->hwndDocument : (HWND) NULL;

   // Check/uncheck the document title in the window menu

      WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
        MPFROM2SHORT (pinst->idInternal, FALSE),
        MPFROM2SHORT (MIA_CHECKED, (fActivate)? MIA_CHECKED : FALSE));

   // For maximized documents, insert/remove the maximized child system
   // menu from the desktop and set/reset the desktop title

      if (pinst->usState == SWP_MAXIMIZE)
        {if (fActivate)
           {if (! (BOOL) SHORT1FROMMR (WinSendMsg (pinstDesktop->hwndMenu, MM_QUERYITEM,
              MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) &mi)))
              {WinSendMsg (pinstDesktop->hwndMenu, MM_INSERTITEM, (MPARAM) &(pinstDesktop->miSysMenu), (MPARAM) NULL);
               pinstDesktop->miWindow.iPosition++;
              }
            if ((pinst->iName == 0) || (pinst->szTitleText[0]))
              {sprintf (szName, "%s - %s", (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName,
                (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName);
              }
            else
              {sprintf (szName, "%s - %s:%d", (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName,
                (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName, pinst->iName);
              }
            WinSetWindowText (pinstDesktop->hwndFrame, szName);
           }
         else
           {if ((BOOL) SHORT1FROMMR (WinSendMsg (pinstDesktop->hwndMenu, MM_QUERYITEM,
              MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) &mi)))
              {WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM,
                 MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) NULL);
               pinstDesktop->miWindow.iPosition--;
              }
            WinSetWindowText (pinstDesktop->hwndFrame, (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName);
           }
        }

   // Disable the menu to avoid flicker

      WinEnableWindow (pinstDesktop->hwndMenu, FALSE);

   // If we are activating the document, remove the old menu delta

      if (fActivate && pinstDesktop->hwndMenuDelta && (pinstDesktop->hwndMenuDelta != pinst->hwndMenu))
        {for (iItem = 0; iItem < pinstDesktop->cidDelta; iItem++)
            WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM, MPFROM2SHORT (pinstDesktop->idDelta[iItem], TRUE), (MPARAM) NULL);
         pinstDesktop->cidDelta = 0;
        }

   // If we are activating the document, insert the new menu delta

      if (fActivate && pinst->hwndMenu && (pinstDesktop->hwndMenuDelta != pinst->hwndMenu))
        {iItem = 0;
         while ((mi.id = SHORT1FROMMR (WinSendMsg (pinst->hwndMenu, MM_ITEMIDFROMPOSITION, MPFROMSHORT (iItem), (MPARAM) NULL))) != MID_ERROR)
           {WinSendMsg (pinst->hwndMenu, MM_QUERYITEMTEXT, MPFROM2SHORT (mi.id, MAX_STRING), (MPARAM) szName);
            WinSendMsg (pinst->hwndMenu, MM_QUERYITEM, MPFROM2SHORT (mi.id, FALSE), (MPARAM) &mi);
            mi.iPosition = pinstDesktop->miDelta.iPosition + iItem;
            WinSendMsg (pinstDesktop->hwndMenu, MM_INSERTITEM, (MPARAM) &mi, (MPARAM) szName);
            if (iItem < MAX_MENUDELTA)
               pinstDesktop->idDelta[iItem] = mi.id;
            iItem++;
           }
         pinstDesktop->hwndMenuDelta = pinst->hwndMenu;
         pinstDesktop->cidDelta = min (MAX_MENUDELTA, iItem);
        }

   // Reenable the menu

      WinEnableWindow (pinstDesktop->hwndMenu, TRUE);

   // Indicate activate handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentArrange - Arrange all documents in the desktop
// -------------------------------------------------------------------
   VOID FAR MDIDocumentArrange (hwndDesktop, fStyle)

      HWND hwndDesktop;                // Desktop window
      USHORT fStyle;                   // AWP_TILED or AWP_CASCADED

   // Define function data

     {USHORT cswpWnd, cswpIcon;        // Window, Icon count
      PSWP pswpWnd, pswpIcon;          // Window, Icon positions
      ULONG ulStyle;                   // Style flags
      RECTL rcl;                       // Rectangle work area
      HWND hwnd;                       // Window work area
      SHORT cDocs;                     // Document count
      USHORT yIcon;                    // Icon height
      SHORT cxBorderInset;             // Border inset width
      USHORT i;                        // Counter
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      PINSTDOCUMENT pinstDocument;     // -->document instance data
      HWND hwndDocument;               // Document window
      USHORT usNotify;                 // Notification code

      SHORT cxBorder =                 // Border width
        (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);

      SHORT cyBorder =                 // Border height
        (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYSIZEBORDER);

   // Locate the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (hwndDesktop, QWL_USER + sizeof (PVOID));

   // Count the number of document windows

      for (cDocs = 0, hwnd = WinQueryWindow (hwndDesktop, QW_TOP, FALSE);
        hwnd; hwnd = WinQueryWindow (hwnd, QW_NEXT, FALSE)) cDocs++;

   // Allocate space for document and icon positions

      pswpWnd = (PSWP) AllocMemAlloc ((LONG) (sizeof(SWP) * cDocs));
      pswpIcon = (PSWP) AllocMemAlloc ((LONG) (sizeof(SWP) * cDocs));

   // Enumerate windows and selectively add them to the arrange lists

      for (cswpWnd = cswpIcon = 0, hwnd = WinQueryWindow (hwndDesktop, QW_TOP, FALSE);
       hwnd; hwnd = WinQueryWindow (hwnd, QW_NEXT, FALSE))

      // Make sure the window is visible and owned by the desktop

        {ulStyle = WinQueryWindowULong (hwnd, QWL_STYLE);
         if (WinQueryWindow (hwnd, QW_OWNER, FALSE)
          || (WinQueryWindowUShort (hwnd, QWS_ID) == 0)
          || !(ulStyle & WS_VISIBLE)) continue;

      // Count icons

         if (ulStyle & WS_MINIMIZED)
            pswpIcon[cswpIcon++].hwnd = hwnd;

      // Count windows (restore any that are maximized)

         else
           {if (ulStyle & WS_MAXIMIZED)
              {hwndDocument = WinWindowFromID (hwnd, FID_CLIENT);
               pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong
                 (hwndDocument, QWL_USER + sizeof (PVOID));
               WinSetParent (pinstDocument->hwndMinMax, pinstDocument->hwndFrame, FALSE);
               WinSetWindowPos (hwnd, (HWND) NULL, 0, 0, 0, 0, SWP_RESTORE);
               if (pinstDesktop->hwndDocument != hwndDocument)
                  MDIDocumentShowFrameCtls (pinstDocument, FALSE, FALSE, TRUE);
              }
            pswpWnd[cswpWnd++].hwnd = hwnd;
           }

      // Loop until all windows accounted for

        }

   // Get dimensions of desktop window

      WinQueryWindowRect (hwndDesktop, &rcl);
      cxBorderInset = (SHORT) (WinQuerySysValue (HWND_DESKTOP, SV_CXBYTEALIGN)
        - WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER));
      WinInflateRect ((HAB) NULL, &rcl, -cxBorderInset, -cxBorderInset * (cyBorder / cxBorder));

   // Make room for a single row of icons

      if (cswpIcon > 0)
        {yIcon = LOUSHORT (WinQuerySysValue(HWND_DESKTOP, SV_CYICON));
         rcl.yBottom += (yIcon * ICON_PARK_NUM) / ICON_PARK_DENOM;
        }

   // Set window positions

      if (fStyle == AWP_CASCADED)
         MDIDocumentArrangeCascaded (&rcl, cswpWnd, pswpWnd);
      else if (fStyle == AWP_TILED)
         MDIDocumentArrangeTiled (&rcl, cswpWnd, pswpWnd);

   // Set icon positions

      for (i = 0; i < cswpIcon; i++)
        {pswpIcon[i].x = 0;
         pswpIcon[i].y = 0;
         pswpIcon[i].fs = SWP_MOVE;
        }

   // Inform all windows that they are being tiled or cascaded

      for (i = 0; i < cswpWnd; i++)
       {if (fStyle == AWP_CASCADED)
          usNotify = MDI_NOTIFY_CASCADE_INSTANCE;
        else if (fStyle == AWP_TILED)
          usNotify = MDI_NOTIFY_TILE_INSTANCE;
        pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong
          (WinWindowFromID (pswpWnd[i].hwnd, FID_CLIENT), QWL_USER + sizeof (PVOID));
        WinSendMsg (pinstDocument->hwndDocument, WM_CONTROL,
          MPFROM2SHORT (pinstDocument->idDesktop, usNotify), (MPARAM) pinstDocument->pvInstData);
       }

   // Reposition all windows and icons

      WinSetMultWindowPos ((HAB) NULL, pswpWnd, cswpWnd);
   // WinSetMultWindowPos ((HAB) NULL, pswpIcon, cswpIcon);

   // Free space used for document and icon positions

      AllocMemFree (pswpWnd);
      AllocMemFree (pswpIcon);

     }


// Function: MDIDocumentArrangeCascaded - Arrange all documents cascaded
// ---------------------------------------------------------------------
   VOID FAR MDIDocumentArrangeCascaded (prc, cWnd, aswp)

      PRECTL prc;                      // -->area in which arrange will occur
      SHORT cWnd;                      // Count of windows
      PSWP aswp;                       // Array of window positions

   // Define function data

     {SHORT xEdge, yEdge;
      SHORT xDelta, yDelta;
      SHORT cMaxWnd;
      SHORT x, y, i, j;
      RECTL rc;

      SHORT cxBorder =
        (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);

      SHORT cyBorder =
        (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYSIZEBORDER);

   // Set cascade parameters

      rc.xLeft = prc->xLeft - cxBorder;
      rc.xRight = prc->xRight + cyBorder;
      rc.yBottom = prc->yBottom - cyBorder;
      rc.yTop = prc->yTop + cyBorder;

   // Get x and y deltas from system values

      xDelta = LOUSHORT(WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER))
        + LOUSHORT(WinQuerySysValue(HWND_DESKTOP, SV_CXMINMAXBUTTON)) / 2 + 2;
      yDelta = LOUSHORT(WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR));

   // Get initial cut at yEdge using fraction

      yEdge = (((SHORT)(rc.yTop - rc.yBottom)) * CASC_EDGE_NUM) / CASC_EDGE_DENOM;

   // Determine maximum number of deltas used per run

      cMaxWnd = (((SHORT)(rc.yTop - rc.yBottom)) - yEdge) / yDelta;

   // Set x and y edges so full cascade will fill rectangle completely

      xEdge = ((SHORT)(rc.xRight - rc.xLeft)) - xDelta/2 - cMaxWnd * xDelta;
      yEdge = ((SHORT)(rc.yTop - rc.yBottom)) - cMaxWnd * yDelta;
      cMaxWnd++;

   // Arrange if only one run is needed

      if (cWnd <= cMaxWnd)
        {x = (SHORT)rc. xLeft;
         y = (SHORT)rc. yTop - yEdge;
         for (i = cWnd - 1; i >= 0; i--)
           {aswp[i].x = x;
            aswp[i].y = y;
            aswp[i].cx = xEdge;
            aswp[i].cy = yEdge;
            aswp[i].fs = SWP_SIZE | SWP_MOVE;
            x += xDelta;
            y -= yDelta;
           }
        }

   // Arrange if multiple runs are necessary; start at bottom
   // right, iterate up to top left

      else
        {i = 0;
         while (i < cWnd)
           {x = ((SHORT)rc. xLeft) + (cMaxWnd-1) * xDelta;
            y = ((SHORT)rc. yTop) - yEdge - (cMaxWnd-1) * yDelta;
            for (j = 0; j < cMaxWnd; j++)
              {aswp[i].x = x;
               aswp[i].y = y;
               aswp[i].cx = xEdge;
               aswp[i].cy = yEdge;
               aswp[i].fs = SWP_SIZE | SWP_MOVE;
               x -= xDelta;
               y += yDelta;
               if (++i >= cWnd) break;
              }
            if (i >= cWnd) break;
            x = ((SHORT)rc. xLeft) + (cMaxWnd-1) * xDelta + xDelta/2;
            y = ((SHORT)rc. yTop) - yEdge - (cMaxWnd-1) * yDelta + yDelta/2;
            for (j = 0; j < cMaxWnd - 1; j++)
              {aswp[i].x = x;
               aswp[i].y = y;
               aswp[i].cx = xEdge;
               aswp[i].cy = yEdge;
               aswp[i].fs = SWP_SIZE | SWP_MOVE;
               x -= xDelta;
               y += yDelta;
               if (++i >= cWnd) break;
              }
           }
        }
     }


// Function: MDIDocumentArrangeTiled - Arrange all documents tiled
// ---------------------------------------------------------------
   VOID FAR MDIDocumentArrangeTiled (prc, cWnd, aswp)

      PRECTL prc;                      // -->area in which arrange will occur
      SHORT cWnd;                      // Count of windows
      PSWP aswp;                       // Array of window positions

   // Define function data

     {USHORT usRoot;
      SHORT cExtras;
      SHORT iChange;
      SHORT cDiff;
      SHORT x, y, cx, cy;
      SHORT iRow, iCol;

      SHORT cxBorder =
        (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXSIZEBORDER);

      SHORT cyBorder =
        (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYSIZEBORDER);

   // Get grid dimensions

      if ((USHORT) cWnd > (0xFE * 0xFE))
         usRoot = 0x00FF;
      else for (usRoot = 0; (usRoot * usRoot) < (USHORT) cWnd; usRoot++);
      cExtras = usRoot * usRoot - cWnd;

   // Find column where number of rows increases and find initial
   // difference of rows versus columns

      if (cExtras >= (SHORT) usRoot)
        {iChange = cExtras - (SHORT) usRoot;
         cDiff = 2;
        }
      else
        {iChange = cExtras;
         cDiff = 1;
        }

   // Assign x coordinates

      x = (SHORT)prc->xLeft;
      cx = 0;
      for (iCol = 0; iCol < (SHORT) usRoot; iCol++)
        {x += cx - cxBorder;
         cx = ((SHORT)prc->xLeft) + (((SHORT)(prc->xRight - prc->xLeft))
           * (iCol + 1)) / usRoot - x + cxBorder;
         for (iRow = 0; iRow < (SHORT) usRoot - cDiff; iRow++)
           {aswp[iRow * usRoot + iCol].x = x;
            aswp[iRow * usRoot + iCol].cx = cx;
            aswp[iRow * usRoot + iCol].fs = SWP_SIZE | SWP_MOVE;
           }
         if (iCol >= iChange)
           {aswp[iRow * usRoot + iCol - iChange].x = x;
            aswp[iRow * usRoot + iCol - iChange].cx = cx;
            aswp[iRow * usRoot + iCol - iChange].fs = SWP_SIZE | SWP_MOVE;
           }
        }

   // Assign y coordinates, for columns without extra row

      y = (SHORT)prc->yBottom;
      cy = 0;
      for (iRow = usRoot - cDiff - 1; iRow >= 0; iRow--)
        {y += cy - cyBorder;
         cy = ((SHORT)prc->yBottom) + (((SHORT)(prc->yTop - prc->yBottom))
           * (usRoot - cDiff - iRow)) / (usRoot - cDiff) - y + cyBorder;
         for (iCol = 0; iCol < iChange; iCol++)
           {aswp[iRow * usRoot + iCol].y = y;
            aswp[iRow * usRoot + iCol].cy = cy;
           }
        }

   // Assign y coordinates, for columns with extra row
   // do last row first (different offsets)

      y = (SHORT)prc->yBottom - cyBorder;
      cy = ((SHORT)(prc->yTop - prc->yBottom)) / (usRoot - cDiff + 1) + (2 * cyBorder);
      for (iCol = iChange; iCol < (SHORT) usRoot; iCol++)
        {aswp[usRoot * (usRoot - cDiff) + iCol - iChange].y = y;
         aswp[usRoot * (usRoot - cDiff) + iCol - iChange].cy = cy;
        }
      for (iRow = usRoot - cDiff - 1; iRow >= 0; iRow--)
        {y += cy - cyBorder;
         cy = ((SHORT)(prc->yBottom)) + (((SHORT)(prc->yTop - prc->yBottom))
           * (usRoot - cDiff - iRow + 1)) / (usRoot - cDiff + 1) - y + cyBorder;
         for (iCol = iChange; iCol < (SHORT) usRoot; iCol++)
           {aswp[iRow * usRoot + iCol].y = y;
            aswp[iRow * usRoot + iCol].cy = cy;
           }
        }
     }


// Function: MDIDocumentClone - Clone MDI Document
// -----------------------------------------------
   VOID FAR MDIDocumentClone (pinst)

      PINSTDOCUMENT pinst;             // -->document instance data

   // Define function data

     {PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      HWND hwndDocument;               // New document window
      HWND hwndFrame;                  // New document frame
      HPOINTER hptrIcon;               // New document icon
      ULONG flFrameFlags;              // Frame flags
      USHORT usID;                     // Document id

   // Locate the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop,
        QWL_USER + sizeof (PVOID));

   // Copy creation flags from base document; set defaults if missing

      if (pinst->flCreateFlags != (ULONG) NULL)
        flFrameFlags = pinst->flCreateFlags;
      else flFrameFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER | FCF_MINMAX
        | FCF_SHELLPOSITION | FCF_NOBYTEALIGN;

   // Copy QWS_ID from base document

      usID = WinQueryWindowUShort (pinst->hwndDocument, QWS_ID);

   // Create new document

      hwndFrame = MDICreateDocument (pinst->pfnUser, pinstDesktop->hwndDesktop,
        flFrameFlags, pinst->szName, pinst->hmodResources, pinst->idResources,
        &hwndDocument, pinst->cbDocData, pinst->cbInstData, usID);

   // Copy QWL_USER from base document

      WinSetWindowULong (hwndDocument, QWL_USER, WinQueryWindowULong
        (pinst->hwndDocument, QWL_USER));

   // If applicable, copy icon from base document

      if ((hptrIcon = (HPOINTER) WinSendMsg (pinst->hwndFrame,
        WM_QUERYICON, (MPARAM) NULL, (MPARAM) NULL)) != NULL)
         WinSendMsg (hwndFrame, WM_SETICON, (MPARAM) hptrIcon, (MPARAM) NULL);

     }


// Function: MDIDocumentClose - Close MDI Document
// -----------------------------------------------
   MRESULT FAR MDIDocumentClose (pinst)

      PINSTDOCUMENT pinst;             // -->document instance data

   // Destroy document window if the user lets us

     {if ((BOOL) SHORT1FROMMR (WinSendMsg (pinst->hwndDocument, WM_CONTROL,
       MPFROM2SHORT (pinst->idDesktop, MDI_INHIBIT_CLOSE_INSTANCE),
       (MPARAM) pinst->pvInstData)) == FALSE)
         WinDestroyWindow (pinst->hwndFrame);

   // Indicate close handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentCommand - Handle MDI Document Command
// ----------------------------------------------------------
   MRESULT FAR MDIDocumentCommand (pinst, usCommand)

      PINSTDOCUMENT pinst;             // -->document instance data
      USHORT usCommand;                // WM_COMMAND code

   // Act upon command code

     {switch (usCommand)

   // Handle Desktop system menu command by passing it to the desktop frame

        {case CMD_DESKTOP_SIZE:
         case CMD_DESKTOP_MOVE:
         case CMD_DESKTOP_MINIMIZE:
         case CMD_DESKTOP_MAXIMIZE:
         case CMD_DESKTOP_CLOSE:
         case CMD_DESKTOP_NEXT:
         case CMD_DESKTOP_APPMENU:
         case CMD_DESKTOP_SYSMENU:
         case CMD_DESKTOP_RESTORE:
            WinSendMsg (pinst->hwndDesktop, WM_COMMAND,
              MPFROMSHORT (usCommand), MPFROM2SHORT (CMDSRC_ACCELERATOR, FALSE));
            break;

   // Handle Document system menu command by passing the system equivalent
   // to the document's frame. However, if the document is maximized,
   // the command is passed to the desktop for processing

         case CMD_DOCUMENT_SIZE:
         case CMD_DOCUMENT_MOVE:
         case CMD_DOCUMENT_MINIMIZE:
         case CMD_DOCUMENT_MAXIMIZE:
         case CMD_DOCUMENT_CLOSE:
         case CMD_DOCUMENT_NEXT:
         case CMD_DOCUMENT_APPMENU:
         case CMD_DOCUMENT_SYSMENU:
         case CMD_DOCUMENT_RESTORE:
            if (pinst->usState == SWP_MAXIMIZE)
               WinSendMsg (pinst->hwndDesktop, WM_COMMAND,
                 MPFROMSHORT (usCommand), MPFROM2SHORT (CMDSRC_ACCELERATOR, FALSE));
            else
               WinSendMsg (pinst->hwndFrame, WM_SYSCOMMAND,
                 MPFROMSHORT ((usCommand - CMD_DOCUMENT_BASE) | SC_SIZE),
                 MPFROM2SHORT (CMDSRC_ACCELERATOR, FALSE));
            break;

   // Terminate command processing

        }

   // Indicate command handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentCreate - Create MDI Document
// -------------------------------------------------
   MRESULT FAR MDIDocumentCreate (hwndDocument, pdoccp)

      HWND hwndDocument;               // Document window
      PDOCUMENTCREATEPARMS pdoccp;     // -->document creation parameters

   // Define function data

     {PINSTDOCUMENT pinst;             // -->instance data
      PINSTDOCUMENT pinstFirst;        // -->first document instance data
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      MENUITEM mi;                     // Menu structure
      CHAR szClassName[MAX_STRING];    // Class name
      BOOL fIniDatFound;               // TRUE == OS2.INI data found
      USHORT cbBuf;                    // Length of OS2.INI data
      HWND hwndSibling;                // Sibling window handle
      SWP swp;                         // Sibling position

   // Acquire instance data and save in MDI document window

      pinst = AllocMemAlloc ((LONG) (sizeof (*pinst)));
      WinSetWindowULong (hwndDocument, QWL_USER + sizeof (PVOID), (ULONG) pinst);

   // Copy document creation parameters if available

      if (pdoccp != (PDOCUMENTCREATEPARMS) NULL)
        {pinst->cbDocData = pdoccp->cbDocData;
         pinst->cbInstData = pdoccp->cbInstData;
         pinst->pfnUser = pdoccp->pfnUser;
         pinst->flCreateFlags = pdoccp->flCreateFlags;
         pinst->hmodResources = pdoccp->hmodResources;
         pinst->idResources = pdoccp->idResources;
        }

   // Initialize inter-window relationships

      pinst->hwndDocument = hwndDocument;
      pinst->hwndFrame = WinQueryWindow (pinst->hwndDocument, QW_PARENT, FALSE);
      pinst->hwndDesktop = WinQueryWindow (pinst->hwndFrame, QW_PARENT, FALSE);
      pinst->hwndMenu = WinWindowFromID (pinst->hwndFrame, FID_MENU);
      pinst->hwndSysMenu = WinWindowFromID (pinst->hwndFrame, FID_SYSMENU);
      pinst->hwndTitleBar = WinWindowFromID (pinst->hwndFrame, FID_TITLEBAR);
      pinst->hwndMinMax = WinWindowFromID (pinst->hwndFrame, FID_MINMAX);
      pinst->hwndVScroll = WinWindowFromID (pinst->hwndFrame, FID_VERTSCROLL);
      pinst->hwndHScroll = WinWindowFromID (pinst->hwndFrame, FID_HORZSCROLL);
      pinst->idDesktop = WinQueryWindowUShort (WinQueryWindow (pinst->hwndDesktop, QW_PARENT, FALSE), QWS_ID);

   // Verify inter-window relationships

      WinQueryClassName (pinst->hwndDesktop, sizeof (szClassName), szClassName);
      while (stricmp (szClassName, MDI_DESKTOPCLASS) != 0)
         TellUser (ERROR_DOCUMENT_PARENT_INVALID, MDI_MODNAME, MB_ABORTRETRYIGNORE | MB_ICONHAND);

      while (pinst->hwndSysMenu == (HWND) NULL)
         TellUser (ERROR_DOCUMENT_NO_SYSMENU, MDI_MODNAME, MB_ABORTRETRYIGNORE | MB_ICONHAND);

   // Find the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // Load accelerator table

      pinst->haccel = MDILoadAccelTable (ACCEL_DOCUMENT);
      WinSetAccelTable ((HAB) NULL, pinst->haccel, pinst->hwndFrame);

   // Make sure that the menu bar (if any) is not displayed

      if (pinst->hwndMenu != (HWND) NULL)
        {WinSetParent (pinst->hwndMenu, HWND_OBJECT, FALSE);
         WinSetOwner (pinst->hwndMenu, pinst->hwndDesktop);
         WinSendMsg (pinst->hwndFrame, WM_UPDATEFRAME, MPFROMSHORT (FCF_MENU), (MPARAM) NULL);
        }

   // Alter the appearance of the child system menu to the MDI standard

      WinSendMsg (pinst->hwndSysMenu, MM_QUERYITEM, MPFROM2SHORT (SC_SYSMENU, FALSE), (MPARAM) &mi);
      WinSendMsg (pinst->hwndSysMenu, MM_REMOVEITEM, MPFROM2SHORT (SC_SYSMENU, FALSE), (MPARAM) NULL);
      mi.hItem = (ULONG) WinGetSysBitmap (HWND_DESKTOP, SBMP_CHILDSYSMENU);
      mi.hwndSubMenu = MDILoadMenu (pinst->hwndSysMenu, MENU_CHILD_SYSMENU);
      WinSendMsg (pinst->hwndSysMenu, MM_INSERTITEM, (MPARAM) &mi, (MPARAM) NULL);

   // Determine initial title

      WinQueryWindowText (pinst->hwndFrame, sizeof (pinst->szName), pinst->szName);

   // Rebuild the titles of all document windows inserting the current title

      pinstFirst = MDIDocumentTitlesRebuild (pinst, INSERT_TITLE);

   // If applicable, allocate user document and instance data

      if (pdoccp && (pdoccp->cbInstData > 0))
         pinst->pvInstData = AllocMemAlloc ((LONG) (pdoccp->cbInstData));
      if (pdoccp && (pdoccp->cbDocData > 0))
        {if (pinst == pinstFirst)
            pinst->pvDocData = AllocMemAlloc ((LONG) (pdoccp->cbDocData));
         else if (pinstFirst != NULL)
            pinst->pvDocData = pinstFirst->pvDocData;
        }

   // Recall window information from OS2.INI file, if available

      cbBuf = sizeof (pinst->swp);
      pinst->swp.fs = 0;
      if (! pinstDesktop->fDisableDocumentPosition)
        fIniDatFound = WinQueryProfileData ((HAB) NULL, MDI_DOCUMENTCLASS,
          pinst->szName, &(pinst->swp), &cbBuf);
      else fIniDatFound = FALSE;

   // If no prior position was stored, or if the window was maximized,
   // minimized or hidden, find a default position inside the desktop
   // window. If there is a sibling that is a frame, and is itself not
   // maximized, minimized or hidden, offset the new window from it

      if (! fIniDatFound || (pinst->swp.fs & (SWP_MINIMIZE | SWP_MAXIMIZE /* | SWP_HIDE */)))
        {WinQueryWindowPos (pinst->hwndDesktop, &swp);
         pinst->swp.cx = swp.cx / 2;
         pinst->swp.cy = swp.cy / 2;
         pinst->swp.x = swp.x + ((SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXVSCROLL) / 2);
         pinst->swp.y = swp.y + pinst->swp.cy - ((SHORT) WinQuerySysValue
           (HWND_DESKTOP, SV_CYTITLEBAR) / 2);
         if ((hwndSibling = WinQueryWindow (pinst->hwndFrame, QW_NEXT, FALSE)) != (HWND) NULL)
           {WinQueryClassName (hwndSibling, sizeof (szClassName), szClassName);
            if (strcmpi (szClassName, "#1") == 0)
              {WinQueryWindowPos (hwndSibling, &swp);
               if (! (swp.fs & (SWP_MINIMIZE | SWP_MAXIMIZE /* | SWP_HIDE */)))
                 {pinst->swp.x = swp.x + ((SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CXVSCROLL) / 2);
                  pinst->swp.y = swp.y - (SHORT) WinQuerySysValue (HWND_DESKTOP, SV_CYTITLEBAR);
                  pinst->swp.cx = swp.cx;
                  pinst->swp.cy = swp.cy;
                 }
              }
           }
         pinst->swp.fs &= ~SWP_HIDE;
        }

   // Re-tile or re-cascade all windows if necessary or simply
   // display and position document window

      if (pinstDesktop->fTileAlways)
         MDIDocumentArrange (pinst->hwndDesktop, AWP_TILED);
      else if (pinstDesktop->fCascadeAlways)
         MDIDocumentArrange (pinst->hwndDesktop, AWP_CASCADED);
      else
        {pinst->swp.fs |= SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ZORDER;
         pinst->swp.hwndInsertBehind = HWND_TOP;
         pinst->swp.hwnd = pinst->hwndFrame;
         WinSetMultWindowPos ((HAB) NULL, &(pinst->swp), 1);
        }

   // Force the current window to be activated

      MDIDocumentActivate (pinst, TRUE);

   // Indicate create handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentDestroy - Destroy MDI Document
// ---------------------------------------------------
   MRESULT FAR MDIDocumentDestroy (pinst)

      PINSTDOCUMENT pinst;             // -->document instance data

   // Define function data

     {PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      PINSTDOCUMENT pinstFirst;        // -->first document instance data
      PINSTDOCUMENT pinstDocument;     // -->next active document instance data
      HWND hwndFrame;                  // Next active document frame
      MENUITEM mi;                     // Menu item

   // Find the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // Save window position onto OS2.INI file

      pinst->swp.fs &= ~SWP_HIDE;
      if (! pinstDesktop->fDisableDocumentPosition)
        WinWriteProfileData ((HAB) NULL, MDI_DOCUMENTCLASS, pinst->szName,
          &(pinst->swp), sizeof (pinst->swp));

   // Rebuild the titles of all document windows removing the current title

      pinstFirst = MDIDocumentTitlesRebuild (pinst, REMOVE_TITLE);

   // Free accelerator table

      WinDestroyAccelTable (pinst->haccel);

   // Re-tile or re-cascade all remaining windows if necessary

      if (pinstDesktop->fTileAlways)
         MDIDocumentArrange (pinst->hwndDesktop, AWP_TILED);
      else if (pinstDesktop->fCascadeAlways)
         MDIDocumentArrange (pinst->hwndDesktop, AWP_CASCADED);

   // If there is a next entry in the window submenu, activate it

      if ((mi.id = SHORT1FROMMR (WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_ITEMIDFROMPOSITION,
       MPFROMSHORT (CMD_WINDOW_MAX - CMD_WINDOW - pinstDesktop->fDisableNewDocument), (MPARAM) NULL))) > 0)
        {hwndFrame = MDIDocumentFrameFromID (pinstDesktop->hwndDesktop, mi.id);
         WinSetWindowPos (hwndFrame, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);
         pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (WinWindowFromID (hwndFrame, FID_CLIENT), QWL_USER + sizeof (PVOID));
         if (pinstDocument)
            MDIDocumentActivate (pinstDocument, TRUE);
        }

   // Tell user instance and maybe document will be destroyed

      WinSendMsg (pinst->hwndDocument, WM_CONTROL,
        MPFROM2SHORT (pinst->idDesktop, MDI_NOTIFY_DESTROY_INSTANCE),
        (MPARAM) pinst->pvInstData);
      if (pinstFirst == (PINSTDOCUMENT) NULL)
         WinSendMsg (pinst->hwndDocument, WM_CONTROL,
           MPFROM2SHORT (pinst->idDesktop, MDI_NOTIFY_DESTROY_DOCUMENT), (MPARAM) pinst->pvDocData);

   // If applicable, free the user document and instance data

      if (pinst->pvInstData)
         AllocMemFree (pinst->pvInstData);
      if (pinst->pvDocData && (pinstFirst == (PINSTDOCUMENT) NULL))
         AllocMemFree (pinst->pvDocData);

   // Free instance data

      AllocMemFree (pinst);

   // Indicate destroy handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentFrameFromID - Return Document Frame From Its Internal ID
// -----------------------------------------------------------------------------
   HWND FAR MDIDocumentFrameFromID (hwndDesktop, idInternal)

      HWND hwndDesktop;                // Desktop window
      USHORT idInternal;               // Requested internal id

   // Define function data

     {PINSTDOCUMENT pinstDocument;     // -->document instance data
      HWND hwndFrame;                  // Document frame

   // Scan for a match on internal id

      for (hwndFrame = WinQueryWindow (hwndDesktop, QW_TOP, FALSE);
       hwndFrame; hwndFrame = WinQueryWindow (hwndFrame, QW_NEXT, FALSE))
        {if ((WinQueryWindow (hwndFrame, QW_OWNER, FALSE) == (HWND) NULL)
          && (WinQueryWindowUShort (hwndFrame, QWS_ID) != 0))
           {pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong
              (WinWindowFromID (hwndFrame, FID_CLIENT), QWL_USER + sizeof (PVOID));
            if (pinstDocument->idInternal == idInternal) break;
           }
        }

   // Return to caller

      return hwndFrame;

     }


// Function: MDIDocumentHide - Hide MDI Document
// ---------------------------------------------
   VOID FAR MDIDocumentHide (pinst)

      PINSTDOCUMENT pinst;             // -->document instance data

   // Define function data

     {PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      PINSTDOCUMENT pinstDocument;     // -->next active document instance data
      HWND hwndFrame;                  // Next active document frame
      MENUITEM mi;                     // Menu item
      SHORT iItem;                     // Item index

   // Locate the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // Hide document

      WinShowWindow (pinst->hwndFrame, FALSE);
      pinst->usState |= SWP_HIDE;

   // Hide document entry from window submenu

      MDIDocumentTitlesRebuild (pinst, HIDE_TITLE);

   // Delete any remaining menu delta entries

      for (iItem = 0; iItem < (SHORT) pinstDesktop->cidDelta; iItem++)
         WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM, MPFROM2SHORT (pinstDesktop->idDelta[iItem], TRUE), (MPARAM) NULL);
      pinstDesktop->cidDelta = 0;

   // If there is a next entry in the window submenu, activate it

      if ((mi.id = SHORT1FROMMR (WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_ITEMIDFROMPOSITION,
       MPFROMSHORT (CMD_WINDOW_MAX - CMD_WINDOW - pinstDesktop->fDisableNewDocument), (MPARAM) NULL))) > 0)
        {hwndFrame = MDIDocumentFrameFromID (pinstDesktop->hwndDesktop, mi.id);
         WinSetWindowPos (hwndFrame, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);
         pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (WinWindowFromID (hwndFrame, FID_CLIENT), QWL_USER + sizeof (PVOID));
         if (pinstDocument)
            MDIDocumentActivate (pinstDocument, TRUE);
        }

   // Otherwise, remove any maximized child system menu and reset the desktop
   // In addition, we disable the HIDE and NEW window menu options

      else
        {WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM,
           MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) NULL);
         WinSetWindowText (pinstDesktop->hwndFrame, (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName);
         pinstDesktop->hwndDocument = (HWND) NULL;
         WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_HIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED));
         WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_NEW_DOCUMENT, FALSE), MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED));
        }

   // Enable the UNHIDE window submenu option

      WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
        MPFROM2SHORT (CMD_UNHIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, FALSE));

   // Re-tile or re-cascade if necessary

      if (pinstDesktop->fTileAlways)
         MDIDocumentArrange (pinstDesktop->hwndDesktop, AWP_TILED);
      else if (pinstDesktop->fCascadeAlways)
         MDIDocumentArrange (pinstDesktop->hwndDesktop, AWP_CASCADED);

     }


// Function: MDIDocumentMinMax - Minimize/Maximize MDI Document
// ------------------------------------------------------------
   MRESULT FAR MDIDocumentMinMax (pinst, pswp)

      PINSTDOCUMENT pinst;             // -->document instance data
      PSWP pswp;                       // -->document position information

   // Define function data

     {PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      CHAR szName[MAX_STRING];         // Desktop name
      MENUITEM mi;                     // Menu item

   // Update document state flags

      pinst->usState = pswp->fs & (SWP_MAXIMIZE | SWP_MINIMIZE);

   // Show/hide all frame controls

      MDIDocumentShowFrameCtls (pinst, (pinst->usState != SWP_MAXIMIZE), TRUE, FALSE);

   // Locate the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // For maximized documents, insert the maximized child system
   // menu to the desktop if it does not already exist, and update
   // the desktop title

      if (pinst->usState == SWP_MAXIMIZE)
        {if (! (BOOL) SHORT1FROMMR (WinSendMsg (pinstDesktop->hwndMenu, MM_QUERYITEM,
           MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) &mi)))
           {WinSendMsg (pinstDesktop->hwndMenu, MM_INSERTITEM,
              (MPARAM) &(pinstDesktop->miSysMenu), (MPARAM) NULL);
            pinstDesktop->miWindow.iPosition++;
           }
         if ((pinst->iName == 0) || (pinst->szTitleText[0]))
           {sprintf (szName, "%s - %s", (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName,
             (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName);
           }
         else
           {sprintf (szName, "%s - %s:%d", (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName,
             (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName, pinst->iName);
           }
         WinSetWindowText (pinstDesktop->hwndFrame, szName);
        }

   // Otherwise, remove the maximized child system menu from the desktop
   // and reset the desktop title

      else
        {if ((BOOL) SHORT1FROMMR (WinSendMsg (pinstDesktop->hwndMenu, MM_QUERYITEM,
           MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) &mi)))
           {WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM,
              MPFROM2SHORT (MENU_MAXCHILD_SYSMENU, FALSE), (MPARAM) NULL);
            pinstDesktop->miWindow.iPosition--;
           }
         WinSetWindowText (pinstDesktop->hwndFrame, (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName);
        }

   // Indicate min/max handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentMoreDlgProc - MDI Document "More Documents" dialog procedure
// ---------------------------------------------------------------------------------
   MRESULT EXPENTRY MDIDocumentMoreDlgProc (hwndMore, msg, mp1, mp2)

      HWND hwndMore;                   // More dialog box
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Message parameter 1
      MPARAM mp2;                      // Message parameter 2

   // Define function data

     {HWND hwndDocument;               // Document window
      HWND hwndFrame;                  // Its frame
      HWND hwndActive;                 // Document to be made active
      CHAR szClassName[MAX_STRING];    // Class/Title name
      CHAR szName[MAX_STRING];         // Window name
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      PINSTDOCUMENT pinstDocument;     // -->document instance data
      SHORT cmd;                       // Which window to retrieve
      SHORT i;                         // Temporary counter

   // Analyze and process message

      switch (msg)

      // Initially, load list box with all window titles

        {case WM_INITDLG:
            pinstDesktop = (PINSTDESKTOP) mp2;
            WinSetWindowULong (hwndMore, QWL_USER, (ULONG) pinstDesktop);

         // Scan each document window in the desktop

            cmd = QW_TOP;
            hwndFrame = pinstDesktop->hwndDesktop;
            while ((hwndFrame = WinQueryWindow (hwndFrame, cmd, FALSE)) != (HWND) NULL)
              {cmd = QW_NEXT;
               WinQueryClassName (hwndFrame, sizeof (szClassName), szClassName);
               if (strcmpi (szClassName, "#1") != 0) continue;
               hwndDocument = WinWindowFromID (hwndFrame, FID_CLIENT);
               WinQueryClassName (hwndDocument, sizeof (szClassName), szClassName);
               if (strcmpi (szClassName, MDI_DOCUMENTCLASS) != 0) continue;
               pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));

            // Add the document title to the list box

               if (! (pinstDocument->usState & SWP_HIDE))
                 {if ((pinstDocument->iName == 0) || (pinstDocument->szTitleText[0]))
                     sprintf (szClassName, "%s", (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
                  else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);
                  WinSendMsg (WinWindowFromID (hwndMore, DIALOG_MORE_LISTBOX),
                    LM_INSERTITEM, MPFROMSHORT (LIT_SORTASCENDING), (MPARAM) szClassName);
                 }

            // Loop until all documents processed

              }

         // Indicate initialize handled

            return (MRESULT) NULL;

      // When OK hit, make selected entry active

         case WM_COMMAND:
            hwndActive = (HWND) NULL;
            if (SHORT1FROMMR (mp1) == DID_OK)
              {i = SHORT1FROMMR (WinSendDlgItemMsg (hwndMore, DIALOG_MORE_LISTBOX,
                 LM_QUERYSELECTION, MPFROMSHORT (LIT_FIRST), (MPARAM) NULL));
               WinSendDlgItemMsg (hwndMore, DIALOG_MORE_LISTBOX,
                 LM_QUERYITEMTEXT, MPFROM2SHORT (i, sizeof (szName)), (MPARAM) szName);
               pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (hwndMore, QWL_USER);

            // Scan each document window in the desktop

               cmd = QW_TOP;
               hwndFrame = pinstDesktop->hwndDesktop;
               while ((hwndFrame = WinQueryWindow (hwndFrame, cmd, FALSE)) != (HWND) NULL)
                 {cmd = QW_NEXT;
                  WinQueryClassName (hwndFrame, sizeof (szClassName), szClassName);
                  if (strcmpi (szClassName, "#1") != 0) continue;
                  hwndDocument = WinWindowFromID (hwndFrame, FID_CLIENT);
                  WinQueryClassName (hwndDocument, sizeof (szClassName), szClassName);
                  if (strcmpi (szClassName, MDI_DOCUMENTCLASS) != 0) continue;
                  pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));

               // Determine the document title

                  if ((pinstDocument->iName == 0) || (pinstDocument->szTitleText[0]))
                     sprintf (szClassName, "%s", (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
                  else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);

               // If the selected document title matches, make it active

                  if (strcmp (szName, szClassName) == 0)
                     hwndActive = pinstDocument->hwndFrame;

               // Loop until all documents processed

                 }

              }

         // Indicate dialog box terminated

            WinDismissDlg (hwndMore, SHORT1FROMMP (mp1));
            if (hwndActive != (HWND) NULL)
               WinSetWindowPos (hwndActive, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);
            return (MRESULT) NULL;

        }

   // Return to PM

      return WinDefDlgProc (hwndMore, msg, mp1, mp2);

     }


// Function: MDIDocumentPaint - Paint MDI Document
// -----------------------------------------------
   MRESULT FAR MDIDocumentPaint (pinst)

      PINSTDOCUMENT pinst;             // -->document instance data

   // Define function data

     {HPS hps;                         // Presentation space
      RECTL rcl;                       // Update rectangle

   // Paint a SYSCLR_WINDOW background

      hps = WinBeginPaint (pinst->hwndDocument, (HPS) NULL, &rcl);
      WinFillRect (hps, &rcl, SYSCLR_APPWORKSPACE);
      WinEndPaint (hps);

   // Indicate paint handled

      return (MRESULT) NULL;

     }


// Function: MDIDocumentShowFrameCtls - Show or Hide Documernt Frame Controls
// --------------------------------------------------------------------------
   VOID FAR MDIDocumentShowFrameCtls (pinst, fShow, fTitle, fScroll)

      PINSTDOCUMENT pinst;             // -->document instance data
      BOOL fShow;                      // TRUE == show frame controls
      BOOL fTitle;                     // TRUE == handle title bar
      BOOL fScroll;                    // TRUE == handle scroll bars

   // Define function data

     {ULONG flStyle = 0;               // Frame style
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data

   // Locate the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // Show/hide all frame controls

      WinShowWindow (pinst->hwndSysMenu, FALSE);
      WinSetParent (pinst->hwndSysMenu, (fShow)? pinst->hwndFrame : HWND_OBJECT, FALSE);
      if (pinst->hwndSysMenu != (HWND) NULL) flStyle |= FCF_SYSMENU;

      WinShowWindow (pinst->hwndMinMax, FALSE);
      WinSetParent (pinst->hwndMinMax, (fShow)? pinst->hwndFrame : HWND_OBJECT, FALSE);
      if (pinst->hwndMinMax != (HWND) NULL) flStyle |= FCF_MINMAX;

      WinShowWindow (pinst->hwndTitleBar, FALSE);
      if (fTitle)
         WinSetParent (pinst->hwndTitleBar, (fShow)? pinst->hwndFrame : HWND_OBJECT, FALSE);
      else WinSetParent (pinst->hwndTitleBar, pinst->hwndFrame, FALSE);
      if (pinst->hwndTitleBar != (HWND) NULL) flStyle |= FCF_TITLEBAR;

   // Handle scroll bars if necessary

      if (fScroll && (! pinstDesktop->fKeepScrollBars))
        {WinShowWindow (pinst->hwndVScroll, FALSE);
         WinSetParent (pinst->hwndVScroll, (fShow)? pinst->hwndFrame : HWND_OBJECT, FALSE);
         if (pinst->hwndVScroll != (HWND) NULL) flStyle |= FCF_VERTSCROLL;
         WinShowWindow (pinst->hwndHScroll, FALSE);
         WinSetParent (pinst->hwndHScroll, (fShow)? pinst->hwndFrame : HWND_OBJECT, FALSE);
         if (pinst->hwndHScroll != (HWND) NULL) flStyle |= FCF_HORZSCROLL;
        }

   // Update frame window

      WinSendMsg (pinst->hwndFrame, WM_UPDATEFRAME, (MPARAM) flStyle, (MPARAM) NULL);

   // Return

     }


// Function: MDIDocumentTitlesRebuild - Rebuild the titles of all Documents
// ------------------------------------------------------------------------
   PINSTDOCUMENT FAR MDIDocumentTitlesRebuild (pinst, usCommand)

      PINSTDOCUMENT pinst;             // -->document instance data
      USHORT usCommand;                // INSERT_TITLE or REMOVE_TITLE or UPDATE_TITLE or HIDE_TITLE

   // Define function data

     {HWND hwndDocument;               // Document window
      HWND hwndFrame;                  // Its frame
      CHAR szClassName[MAX_STRING];    // Class/Title name
      PINSTDOCUMENT pinstFirst;        // -->first document instance data
      PINSTDOCUMENT pinstDocument;     // -->document instance data
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      MENUITEM mi;                     // Menu item
      SHORT cmd;                       // Which window to retrieve
      SHORT nSameName;                 // Number of documents with the same name
      SHORT nDocuments;                // Number of documents
      SHORT iDocument;                 // Document index
      SHORT cVisDocs;                  // Count of visible documents
      SHORT iItem;                     // Item index
      BOOL fMore;                      // TRUE == "more..." present
      BOOL fListed;                    // TRUE == at least one document listed

   // Find the desktop instance data

      pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));

   // Count the number of documents that already exist with the same name
   // as the current document and the total number of documents

      cmd = QW_TOP;
      hwndFrame = pinst->hwndDesktop;
      nDocuments = nSameName = 0;
      while ((hwndFrame = WinQueryWindow (hwndFrame, cmd, FALSE)) != (HWND) NULL)
        {cmd = QW_NEXT;
         WinQueryClassName (hwndFrame, sizeof (szClassName), szClassName);
         if (strcmpi (szClassName, "#1") != 0) continue;
         hwndDocument = WinWindowFromID (hwndFrame, FID_CLIENT);
         WinQueryClassName (hwndDocument, sizeof (szClassName), szClassName);
         if (strcmpi (szClassName, MDI_DOCUMENTCLASS) != 0) continue;
         pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));
         nDocuments++;
         if (pinst == pinstDocument) continue;
         if (strcmp (pinst->szName, pinstDocument->szName) == 0)
            nSameName++;
        }
      if (usCommand == REMOVE_TITLE) nDocuments--;

   // If there is exactly one document, insert the window submenu
   // if it has not already been created

      if ((nDocuments == 1) && (usCommand == INSERT_TITLE))
        {if (! pinstDesktop->fKeepWindowsMenu)
           WinSendMsg (pinstDesktop->hwndMenu, MM_INSERTITEM, (MPARAM) &(pinstDesktop->miWindow), (MPARAM) pinstDesktop->pszWindow);
        }

   // If there are no documents left, remove the window submenu and any
   // remaining menu delta. If we cannot remove the windows menu item,
   // we disable relevant entries

      else if ((nDocuments == 0) && (usCommand == REMOVE_TITLE))
        {if (! pinstDesktop->fKeepWindowsMenu)
            WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM, MPFROM2SHORT (MENU_WINDOW, FALSE), (MPARAM) NULL);
         for (iItem = 0; iItem < (SHORT) pinstDesktop->cidDelta; iItem++)
            WinSendMsg (pinstDesktop->hwndMenu, MM_REMOVEITEM, MPFROM2SHORT (pinstDesktop->idDelta[iItem], TRUE), (MPARAM) NULL);
         pinstDesktop->cidDelta = 0;
        }

   // Remove all the document entries in the window submenu

      for (iDocument = 0; iDocument <= nDocuments; iDocument++)
        {WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_DELETEITEM,
           MPFROM2SHORT (CMD_SELECT_DOCUMENT + iDocument, FALSE), (MPARAM) NULL);
        }

   // Remove any "more documents" entry

      WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_DELETEITEM,
        MPFROM2SHORT (CMD_MORE_DOCUMENTS, FALSE), (MPARAM) NULL);
      fMore = FALSE;

   // Scan each document window in the desktop

      cmd = QW_TOP;
      hwndFrame = pinst->hwndDesktop;
      iDocument = -1;
      cVisDocs = 0;
      pinstFirst = (PINSTDOCUMENT) NULL;
      fListed = FALSE;
      while ((hwndFrame = WinQueryWindow (hwndFrame, cmd, FALSE)) != (HWND) NULL)
        {cmd = QW_NEXT;
         WinQueryClassName (hwndFrame, sizeof (szClassName), szClassName);
         if (strcmpi (szClassName, "#1") != 0) continue;
         hwndDocument = WinWindowFromID (hwndFrame, FID_CLIENT);
         WinQueryClassName (hwndDocument, sizeof (szClassName), szClassName);
         if (strcmpi (szClassName, MDI_DOCUMENTCLASS) != 0) continue;
         pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));

      // Reset the id of this document

         if ((pinst == pinstDocument) && (usCommand == REMOVE_TITLE))
           {pinstDocument->idInternal =  CMD_SELECT_DOCUMENT - 1;
            continue;
           }
         else if ((usCommand == INSERT_TITLE) || (usCommand == REMOVE_TITLE))
           {iDocument++;
            pinstDocument->idInternal = CMD_SELECT_DOCUMENT + iDocument;
           }

      // If document titles match, re-number title

         if (strcmp (pinst->szName, pinstDocument->szName) == 0)

         // If we are inserting the current title, re-number apropriately

           {if ((usCommand == INSERT_TITLE) || (usCommand == UPDATE_TITLE))
              {if (nSameName == 0)
                 {pinstDocument->iName = 0;
                  if (usCommand == UPDATE_TITLE)
                     WinSetWindowText (pinstDocument->hwndFrame, pinstDocument->szTitleText);
                 }
               else if (nSameName == 1)
                 {pinstDocument->iName = (pinst == pinstDocument)? 2 : 1;
                  if (pinstDocument->szTitleText[0])
                     sprintf (szClassName, "%s", pinstDocument->szTitleText);
                  else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);
                  WinSetWindowText (pinstDocument->hwndFrame, szClassName);
                 }
               else if (pinst == pinstDocument)
                 {pinstDocument->iName = nSameName + 1;
                  if (pinstDocument->szTitleText[0])
                     sprintf (szClassName, "%s", pinstDocument->szTitleText);
                  else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);
                  WinSetWindowText (pinstDocument->hwndFrame, szClassName);
                 }
              }

         // If we are removing the current title, re-number apropriately

            else if (usCommand == REMOVE_TITLE)
              {if (nSameName == 1)
                 {pinstDocument->iName = 0;
                  sprintf (szClassName, "%s", (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
                  WinSetWindowText (pinstDocument->hwndFrame, szClassName);
                 }
               else if ((nSameName > 1) && (pinstDocument->iName > pinst->iName))
                 {pinstDocument->iName--;
                  if (pinstDocument->szTitleText[0])
                     sprintf (szClassName, "%s", pinstDocument->szTitleText);
                  else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);
                  WinSetWindowText (pinstDocument->hwndFrame, szClassName);
                 }
              }

         // Save a pointer to the instance data of the first document
         // with the same name

            if (pinstDocument->iName <= 1)
               pinstFirst = pinstDocument;
           }

      // Put document title into the window submenu if it is not hidden
      // and if there are not already 9 entries

         if (! (pinstDocument->usState & SWP_HIDE) && (cVisDocs < 9))
           {mi.id = pinstDocument->idInternal;
            mi.iPosition = MIT_END;
            mi.afAttribute = 0;
            mi.hwndSubMenu = (HWND) NULL;
            mi.hItem = (ULONG) NULL;
            mi.afStyle = MIS_TEXT;
            if ((pinstDocument->iName == 0) || (pinstDocument->szTitleText[0]))
               sprintf (szClassName, "%s", (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
            else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);
            WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_INSERTITEM, (MPARAM) &mi, (MPARAM) szClassName);
            if ((usCommand == UPDATE_TITLE) && (pinstDesktop->hwndDocument == pinstDocument->hwndDocument))
               WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
                 MPFROM2SHORT (mi.id, FALSE), MPFROM2SHORT (MIA_CHECKED, MIA_CHECKED));
            cVisDocs++;
            fListed = TRUE;
           }

      // Put the "More Documents..." entry in the window submenu
      // if there are more than 9 entries

         else if ((cVisDocs == 9) && !fMore)
           {mi.id = CMD_MORE_DOCUMENTS;
            mi.iPosition = MIT_END;
            mi.afAttribute = 0;
            mi.hwndSubMenu = (HWND) NULL;
            mi.hItem = (ULONG) NULL;
            mi.afStyle = MIS_TEXT;
            MDILoadString (STRING_MORE_DOCUMENTS, sizeof (szClassName), szClassName);
            WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_INSERTITEM, (MPARAM) &mi, (MPARAM) szClassName);
            fMore = TRUE;
           }

      // Loop until all windows processed

        }

   // If at least one document is listed in the window submenu, enable
   // the HIDE and NEW window submenu options

      if (nDocuments > 0)
        {WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_HIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, FALSE));
         WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_NEW_DOCUMENT, FALSE), MPFROM2SHORT (MIA_DISABLED, FALSE));
        }
      else if (pinstDesktop->fKeepWindowsMenu)
        {WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_HIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED));
         WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_UNHIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED));
         WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
           MPFROM2SHORT (CMD_NEW_DOCUMENT, FALSE), MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED));
        }

   // Return pointer to instance data of first document with the same name

      return pinstFirst;

     }


// Function: MDIDocumentUnhideDlgProc - MDI Document Unhide dialog procedure
// -------------------------------------------------------------------------
   MRESULT EXPENTRY MDIDocumentUnhideDlgProc (hwndUnhide, msg, mp1, mp2)

      HWND hwndUnhide;                 // Unhide dialog box
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Message parameter 1
      MPARAM mp2;                      // Message parameter 2

   // Define function data

     {HWND hwndDocument;               // Document window
      HWND hwndFrame;                  // Its frame
      HWND hwndFirst;                  // First document unhidden
      CHAR szClassName[MAX_STRING];    // Class/Title name
      CHAR szName[MAX_STRING];         // Window name
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      PINSTDOCUMENT pinstDocument;     // -->document instance data
      SHORT cmd;                       // Which window to retrieve
      SHORT nItems, nUnhidden;         // Number of items, number unhidden
      SHORT i;                         // Temporary counter

   // Analyze and process message

      switch (msg)

      // Initially, load list box with all hidden window titles

        {case WM_INITDLG:
            pinstDesktop = (PINSTDESKTOP) mp2;
            WinSetWindowULong (hwndUnhide, QWL_USER, (ULONG) pinstDesktop);

         // Scan each document window in the desktop

            cmd = QW_TOP;
            hwndFrame = pinstDesktop->hwndDesktop;
            while ((hwndFrame = WinQueryWindow (hwndFrame, cmd, FALSE)) != (HWND) NULL)
              {cmd = QW_NEXT;
               WinQueryClassName (hwndFrame, sizeof (szClassName), szClassName);
               if (strcmpi (szClassName, "#1") != 0) continue;
               hwndDocument = WinWindowFromID (hwndFrame, FID_CLIENT);
               WinQueryClassName (hwndDocument, sizeof (szClassName), szClassName);
               if (strcmpi (szClassName, MDI_DOCUMENTCLASS) != 0) continue;
               pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));

            // If the document is hidden, add its title to the list box

               if (pinstDocument->usState & SWP_HIDE)
                 {if ((pinstDocument->iName == 0) || (pinstDocument->szTitleText[0]))
                     sprintf (szClassName, "%s", (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
                  else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);
                  WinSendMsg (WinWindowFromID (hwndUnhide, DIALOG_UNHIDE_LISTBOX),
                    LM_INSERTITEM, MPFROMSHORT (LIT_SORTASCENDING), (MPARAM) szClassName);
                 }

            // Loop until all documents processed

              }

         // Indicate initialize handled

            return (MRESULT) NULL;

      // When OK hit, unhide all selected entries

         case WM_COMMAND:
            if (SHORT1FROMMR (mp1) == DID_OK)

         // Scan each selected entry in the list box

              {i = SHORT1FROMMR (WinSendDlgItemMsg (hwndUnhide, DIALOG_UNHIDE_LISTBOX,
                 LM_QUERYSELECTION, MPFROMSHORT (LIT_FIRST), (MPARAM) NULL));
               nItems = SHORT1FROMMR (WinSendDlgItemMsg (hwndUnhide, DIALOG_UNHIDE_LISTBOX,
                 LM_QUERYITEMCOUNT, (MPARAM) NULL, (MPARAM) NULL));
               nUnhidden = 0;
               hwndFirst = (HWND) NULL;
               pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (hwndUnhide, QWL_USER);
               while (i != LIT_NONE)
                 {WinSendDlgItemMsg (hwndUnhide, DIALOG_UNHIDE_LISTBOX,
                    LM_QUERYITEMTEXT, MPFROM2SHORT (i, sizeof (szName)), (MPARAM) szName);

               // Scan each document window in the desktop

                  cmd = QW_TOP;
                  hwndFrame = pinstDesktop->hwndDesktop;
                  while ((hwndFrame = WinQueryWindow (hwndFrame, cmd, FALSE)) != (HWND) NULL)
                    {cmd = QW_NEXT;
                     WinQueryClassName (hwndFrame, sizeof (szClassName), szClassName);
                     if (strcmpi (szClassName, "#1") != 0) continue;
                     hwndDocument = WinWindowFromID (hwndFrame, FID_CLIENT);
                     WinQueryClassName (hwndDocument, sizeof (szClassName), szClassName);
                     if (strcmpi (szClassName, MDI_DOCUMENTCLASS) != 0) continue;
                     pinstDocument = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));

                  // If the document is hidden, extract its title

                     if (pinstDocument->usState & SWP_HIDE)
                       {if ((pinstDocument->iName == 0) || (pinstDocument->szTitleText[0]))
                           sprintf (szClassName, "%s", (pinstDocument->szTitleText[0])? pinstDocument->szTitleText : pinstDocument->szName);
                        else sprintf (szClassName, "%s:%d", pinstDocument->szName, pinstDocument->iName);

                     // If title matches select list box entry, unhide document
                     // and put its entry back in the window menu

                        if (strcmp (szClassName, szName) == 0)
                          {pinstDocument->usState &= ~SWP_HIDE;
                           WinShowWindow (pinstDocument->hwndFrame, TRUE);
                           MDIDocumentTitlesRebuild (pinstDocument, HIDE_TITLE);
                           if (hwndFirst == (HWND) NULL)
                              hwndFirst = pinstDocument->hwndFrame;
                           nUnhidden++;
                           break;
                          }
                       }

                  // Loop until all documents scanned

                    }

               // Loop until all list box entries processed

                  i = SHORT1FROMMR (WinSendDlgItemMsg (hwndUnhide, DIALOG_UNHIDE_LISTBOX,
                    LM_QUERYSELECTION, MPFROMSHORT (i), (MPARAM) NULL));
                 }

            // If all entries have been unhidden, disable the
            // UNHIDE window submenu option

               if (nItems == nUnhidden)
                  WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
                    MPFROM2SHORT (CMD_UNHIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, MIA_DISABLED));

            // If any entries at all have been unhidden, enable the
            // HIDE and NEW window submenu options

               WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
                 MPFROM2SHORT (CMD_HIDE, FALSE), MPFROM2SHORT (MIA_DISABLED, FALSE));
               WinSendMsg (pinstDesktop->miWindow.hwndSubMenu, MM_SETITEMATTR,
                 MPFROM2SHORT (CMD_NEW_DOCUMENT, FALSE), MPFROM2SHORT (MIA_DISABLED, FALSE));

            // Re-tile or re-cascade if necessary

               if (pinstDesktop->fTileAlways)
                  MDIDocumentArrange (pinstDesktop->hwndDesktop, AWP_TILED);
               else if (pinstDesktop->fCascadeAlways)
                  MDIDocumentArrange (pinstDesktop->hwndDesktop, AWP_CASCADED);

            // Activate the first document unhidden

               if (hwndFirst != (HWND) NULL)
                  WinSetWindowPos (hwndFirst, HWND_TOP, 0, 0, 0, 0, SWP_ACTIVATE | SWP_ZORDER);

              }

         // Indicate dialog box terminated

            WinDismissDlg (hwndUnhide, SHORT1FROMMP (mp1));
            return (MRESULT) NULL;

        }

   // Return to PM

      return WinDefDlgProc (hwndUnhide, msg, mp1, mp2);

     }


// Function: MDIDocumentWndProc - MDI Document window procedure
// ------------------------------------------------------------
   MRESULT EXPENTRY MDIDocumentWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Message parameter 1
      MPARAM mp2;                      // Message parameter 2

   // Define function data

     {PINSTDOCUMENT pinst;             // -->instance data
      PINSTDESKTOP pinstDesktop;       // -->desktop instance data
      CHAR szName[MAX_STRING];         // Title text

   // Locate instance data

      pinst = (PINSTDOCUMENT) WinQueryWindowULong (hwndDocument, QWL_USER + sizeof (PVOID));

   // Analyze and process message

      switch (msg)

        {case WM_ACTIVATE:
            return MDIDocumentActivate (pinst, (BOOL) SHORT1FROMMP (mp1));

         case WM_CLOSE:
            return MDIDocumentClose (pinst);

         case WM_COMMAND:
            return MDIDocumentCommand (pinst, LOUSHORT (mp1));

         case WM_CREATE:
            return MDIDocumentCreate (hwndDocument, (PDOCUMENTCREATEPARMS) mp1);

         case WM_DESTROY:
            return MDIDocumentDestroy (pinst);

         case WM_MINMAXFRAME:
            return MDIDocumentMinMax (pinst, (PSWP) mp1);

         case WM_MOVE:
            return (MRESULT) !WinQueryWindowPos (pinst->hwndFrame, &(pinst->swp));

         case WM_PAINT:
            return MDIDocumentPaint (pinst);

         case WM_SIZE:
            return (MRESULT) !WinQueryWindowPos (pinst->hwndFrame, &(pinst->swp));

         case MDI_MSG_LOCATE_DESKTOP_DATA:
            pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));
            return (MRESULT) pinstDesktop->pvDeskData;

         case MDI_MSG_LOCATE_DOCUMENT_DATA:
            return (MRESULT) pinst->pvDocData;

         case MDI_MSG_LOCATE_INSTANCE_DATA:
            return (MRESULT) pinst->pvInstData;

         case MDI_MSG_LOCATE_ACTIVE_DOCUMENT:
            return (MRESULT) hwndDocument;

         case MDI_MSG_LOCATE_DESKTOP_MENU:
            pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));
            return (MRESULT) pinstDesktop->hwndMenu;

         case MDI_MSG_LOCATE_DOCUMENT_MENU:
            return (MRESULT) pinst->hwndMenu;

         case MDI_MSG_SET_DESKTOP_TITLE:
            pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));
            strcpy (pinstDesktop->szTitleText, (PCHAR) mp1);
            if (pinst->usState == SWP_MAXIMIZE)
              {if ((pinst->iName == 0) || (pinst->szTitleText[0]))
                  sprintf (szName, "%s - %s", (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName,
                   (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName);
               else sprintf (szName, "%s - %s:%d", (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName,
                   (pinst->szTitleText[0])? pinst->szTitleText : pinst->szName, pinst->iName);
               WinSetWindowText (pinstDesktop->hwndFrame, szName);
              }
            else WinSetWindowText (pinstDesktop->hwndFrame, pinstDesktop->szTitleText);
            return (MRESULT) NULL;

         case MDI_MSG_SET_INSTANCE_TITLE:
            pinstDesktop = (PINSTDESKTOP) WinQueryWindowULong (pinst->hwndDesktop, QWL_USER + sizeof (PVOID));
            strcpy (pinst->szTitleText, (PCHAR) mp1);
            MDIDocumentTitlesRebuild (pinst, UPDATE_TITLE);
            WinSendMsg (pinstDesktop->hwndDesktop, MDI_MSG_SET_DESKTOP_TITLE,
              (MPARAM) (pinstDesktop->szTitleText[0])? pinstDesktop->szTitleText : pinstDesktop->szName, (MPARAM) NULL);
            return (MRESULT) NULL;

        }

   // Return to PM

      return WinDefWindowProc (hwndDocument, msg, mp1, mp2);

     }


// Function: MDIInitialize - Initialize MDI processing
// ---------------------------------------------------
   VOID EXPENTRY MDIInitialize (hab)

      HAB hab;                         // Application anchor block

   // Define function data

     {static DOSFSRSEM dosfsrs;        // Semaphore to block registration
      static BOOL fRegistered = FALSE; // TRUE if already registered

   // Block the actions that follow such that only one process at a
   // time executes them.

      DosFSRamSemRequest (&dosfsrs, SEM_INDEFINITE_WAIT);

   // Once only, perform initialization.

      if (! fRegistered)

      // Register MDI Desktop window class

        {WinRegisterClass (hab, MDI_DESKTOPCLASS, MDIDesktopWndProc,
           /*CS_PUBLIC |*/ CS_CLIPCHILDREN | CS_SIZEREDRAW | CS_MOVENOTIFY, cbExtraData);

      // Register MDI Document window class

         WinRegisterClass (hab, MDI_DOCUMENTCLASS, MDIDocumentWndProc,
           /*CS_PUBLIC |*/ CS_CLIPCHILDREN | CS_SIZEREDRAW | CS_MOVENOTIFY, cbExtraData);

      // Indicate initialization complete

       /*fRegistered = TRUE;*/
        }

   // Release the block and return

      DosFSRamSemClear (&dosfsrs);
     }


// Function: MDILoadAccelTable - Load an MDI Accelerator Table
// -----------------------------------------------------------
   HACCEL FAR MDILoadAccelTable (idAccelTable)

      USHORT idAccelTable;             // Accelerator table id

   // Define function data

     {HACCEL haccel;                   // Accelerator table
      HMODULE hmod;                    // DLL module handle
      CHAR szDLL[MAX_STRING];          // DLL name
      PCHAR pszDot;                    // Temporary pointer

   // Extract the DLL name from our name

      strcpy (szDLL, MDI_MODNAME);
      if ((pszDot = strrchr (szDLL, '.')) != (PCHAR) NULL)
        *pszDot = 0;

   // Load accelerator table

      DosLoadModule (szDLL, sizeof (szDLL), szDLL, &hmod);
      haccel = WinLoadAccelTable ((HAB) NULL, hmod, idAccelTable);
      DosFreeModule (hmod);

   // Return handle

      return haccel;

     }


// Function: MDILoadDialog - Load an MDI Dialog Box
// ------------------------------------------------
   HWND FAR MDILoadDialog (hwndParent, pfnDlgProc, idDialog, pCreateParms)

      HWND hwndParent;                 // Dialog box parent
      PFNWP pfnDlgProc;                // Dialog box procedure
      USHORT idDialog;                 // Dialog box id
      PVOID pCreateParms;              // Creation parameters

   // Define function data

     {HWND hdlg;                       // Dialog box
      HMODULE hmod;                    // DLL module handle
      CHAR szDLL[MAX_STRING];          // DLL name
      PCHAR pszDot;                    // Temporary pointer

   // Extract the DLL name from our name

      strcpy (szDLL, MDI_MODNAME);
      if ((pszDot = strrchr (szDLL, '.')) != (PCHAR) NULL)
        *pszDot = 0;

   // Load dialog box

      DosLoadModule (szDLL, sizeof (szDLL), szDLL, &hmod);
      hdlg = WinLoadDlg (HWND_DESKTOP, hwndParent, pfnDlgProc, hmod, idDialog, pCreateParms);
      DosFreeModule (hmod);

   // Return handle

      return hdlg;

     }


// Function: MDILoadMenu - Load an MDI Menu
// ----------------------------------------
   HWND FAR MDILoadMenu (hwndParent, idMenu)

      HWND hwndParent;                 // Menu parent
      USHORT idMenu;                   // Menu identifier

   // Define function data

     {HWND hMenu;                      // Menu
      HMODULE hmod;                    // DLL module handle
      CHAR szDLL[MAX_STRING];          // DLL name
      SEL sel;                         // Menu template selector
      PVOID pvmt;                      // Menu template address
      PCHAR pszDot;                    // Temporary pointer

   // Extract the DLL name from our name

      strcpy (szDLL, MDI_MODNAME);
      if ((pszDot = strrchr (szDLL, '.')) != (PCHAR) NULL)
        *pszDot = 0;

   // Load menu

      DosLoadModule (szDLL, sizeof (szDLL), szDLL, &hmod);
      DosGetResource (hmod, RT_MENU, idMenu, &sel);
      pvmt = MAKEP (sel, 0);
      hMenu = WinCreateWindow (hwndParent, WC_MENU, (PSZ) NULL, (ULONG) NULL,
          0, 0, 0, 0, hwndParent, HWND_BOTTOM, idMenu, pvmt, (PVOID) NULL);
      DosFreeSeg (sel);
      DosFreeModule (hmod);

   // Return handle

      return hMenu;

     }


// Function: MDILoadString - Load an MDI String
// --------------------------------------------
   VOID FAR MDILoadString (idString, cbString, pszString)

      USHORT idString;                 // String id
      USHORT cbString;                 // Max size of string text
      PCHAR pszString;                 // -->string text

   // Define function data

     {HMODULE hmod;                    // DLL module handle
      CHAR szDLL[MAX_STRING];          // DLL name
      PCHAR pszDot;                    // Temporary pointer

   // Extract the DLL name from our name

      strcpy (szDLL, MDI_MODNAME);
      if ((pszDot = strrchr (szDLL, '.')) != (PCHAR) NULL)
        *pszDot = 0;

   // Load string

      DosLoadModule (szDLL, sizeof (szDLL), szDLL, &hmod);
      WinLoadString ((HAB) NULL, hmod, idString, cbString, pszString);
      DosFreeModule (hmod);

   // Return to caller


     }
