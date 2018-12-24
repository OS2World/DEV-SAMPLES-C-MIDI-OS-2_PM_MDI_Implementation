// Includes
// --------
   #define INCL_WIN
   #define INCL_GPI

   #include <malloc.h>
   #include <stdio.h>
   #include <os2.h>

   #include "Test.H"
   #include "MDI.H"


// Function Declarations
// ---------------------
   MRESULT EXPENTRY DesktopWndProc (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY WhiteWndProc   (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY BlueWndProc    (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY RedWndProc     (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY PinkWndProc    (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY GreenWndProc   (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY CyanWndProc    (HWND, USHORT, MPARAM, MPARAM);
   MRESULT EXPENTRY YellowWndProc  (HWND, USHORT, MPARAM, MPARAM);

   MRESULT          ReportError    (HAB, PCHAR, USHORT);


// Function: main -- Main message loop
// -----------------------------------
   VOID main (VOID)

   // Define function data

     {HAB hab ;
      HMQ hmq;
      QMSG qmsg ;
      HWND hwndDesktop;

      ULONG flCreateFlags = (FCF_STANDARD | FCF_NOBYTEALIGN) & ~FCF_TASKLIST;

      ULONG flOptions = (MDI_KEEP_WINDOWS_MENU | MDI_INITIAL_MAX_DESKTOP | MDI_INITIAL_TILE_ALWAYS
                           | MDI_DISABLE_DESKTOP_POSITION | MDI_ALWAYS_ON_RESIZE | MDI_DISABLE_DOCUMENT_POSITION);

   // Initialize PM

      hab = WinInitialize (0) ;
      hmq = WinCreateMsgQueue (hab, 0) ;

   // Initialize MDI

      MDIInitialize (hab);

   // Create MDI Desktop

      MDICreateDesktop (DesktopWndProc, flCreateFlags, "MDI Test Program",
        (HMODULE) NULL, IDR_DESKTOP, &hwndDesktop, 100, 1, "Options", "Exit", "~Window", flOptions);

   // Main message loop

      while (WinGetMsg (hab, &qmsg, (HWND) NULL, 0, 0))
         WinDispatchMsg (hab, &qmsg) ;

   // Terminate PM

      WinDestroyMsgQueue (hmq) ;
      WinTerminate (hab) ;
     }


// Function: ReportError -- Show PM Errors
// ---------------------------------------
   MRESULT ReportError (hab, pszFileName, usLineNum)

      HAB hab;
      PCHAR pszFileName;
      USHORT usLineNum;

   // Define function data

     {PERRINFO perri;
      CHAR szCaption[128];
      PSZ pszMsg;
      PUSHORT pusMsg;
      MRESULT mrError = NULL;

   // Obtain error information

      if ((perri = WinGetErrorInfo (hab)) == NULL)
         return mrError;

   // Display error information

      SELECTOROF (pusMsg) = SELECTOROF (perri);
      SELECTOROF (pszMsg) = SELECTOROF (perri);
      OFFSETOF (pusMsg) = perri->offaoffszMsg;
      OFFSETOF (pszMsg) = *pusMsg;
      mrError = perri->idError;
      sprintf (szCaption, "Error %lX in %s at line %d", mrError,
        (pszFileName == NULL)? "Unknown" : pszFileName, usLineNum);
      if (WinMessageBox (HWND_DESKTOP, HWND_DESKTOP, pszMsg, szCaption,
        0, MB_CUACRITICAL | MB_OKCANCEL) == MBID_CANCEL)
         WinPostMsg (NULL, WM_QUIT, NULL, NULL);

   // Free error information and return

      WinFreeErrorInfo (perri);
      return mrError;

     }


// Function: DesktopWndProc -- MDI Desktop Window Procedure
// --------------------------------------------------------
   MRESULT EXPENTRY DesktopWndProc (hwndDesktop, msg, mp1, mp2)

      HWND hwndDesktop;                // Desktop window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HWND hwndDocument;
      ULONG flCreateFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER
        | FCF_MINMAX | FCF_ICON | FCF_SHELLPOSITION | FCF_NOBYTEALIGN
        | FCF_VERTSCROLL | FCF_HORZSCROLL | FCF_MENU;

   // Create documents

      switch (msg)
        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_WHITE:
                  MDICreateDocument (WhiteWndProc, hwndDesktop, flCreateFlags,
                    "White", (HMODULE) NULL, IDR_WHITE, &hwndDocument, 100, 100, 100);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_WHITE);
                  return (MRESULT) NULL;
               case IDM_BLUE:
                  MDICreateDocument (BlueWndProc, hwndDesktop, flCreateFlags,
                    "Blue", (HMODULE) NULL, IDR_BLUE, &hwndDocument, 0, 0, 200);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_BLUE);
                  return (MRESULT) NULL;
               case IDM_RED:
                  MDICreateDocument (RedWndProc, hwndDesktop, flCreateFlags,
                    "Red", (HMODULE) NULL, IDR_RED, &hwndDocument, 0, 0, 300);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_RED);
                  return (MRESULT) NULL;
               case IDM_PINK:
                  MDICreateDocument (PinkWndProc, hwndDesktop, flCreateFlags,
                    "Pink", (HMODULE) NULL, IDR_PINK, &hwndDocument, 0, 0, 400);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_PINK);
                  return (MRESULT) NULL;
               case IDM_GREEN:
                  MDICreateDocument (GreenWndProc, hwndDesktop, flCreateFlags,
                    "Green", (HMODULE) NULL, IDR_GREEN, &hwndDocument, 0, 0, 500);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_GREEN);
                  return (MRESULT) NULL;
               case IDM_CYAN:
                  MDICreateDocument (CyanWndProc, hwndDesktop, flCreateFlags,
                    "Cyan", (HMODULE) NULL, IDR_CYAN, &hwndDocument, 0, 0, 600);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_CYAN);
                  return (MRESULT) NULL;
               case IDM_YELLOW:
                  MDICreateDocument (YellowWndProc, hwndDesktop, flCreateFlags,
                    "Yellow", (HMODULE) NULL, IDR_YELLOW, &hwndDocument, 0, 0, 700);
                  WinSetWindowULong (hwndDocument, QWL_USER, CLR_YELLOW);
                  return (MRESULT) NULL;
              }
            break;

      // Respond to desktop notifications

         case WM_CONTROL:
            switch (HIUSHORT (mp1))
              {case MDI_INHIBIT_CLOSE_DESKTOP:
                  return (MRESULT) (MBID_NO == WinMessageBox (HWND_DESKTOP, hwndDesktop,
                    "Do you really want to terminate?", "MDI Desktop", 0, MB_ICONQUESTION | MB_YESNO));
              }
            break;
        }

   // Allow MDI to process all other messages

      return MDIDesktopWndProc (hwndDesktop, msg, mp1, mp2) ;

     }


// Function: WhiteWndProc -- "White" Document Window Procedure
// -----------------------------------------------------------
   MRESULT EXPENTRY WhiteWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;
      PVOID pv;

      static BOOL fChecked;
      static BOOL fGrayed;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "White", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "White", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "White", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinSendMsg (hwndDocument, MDI_MSG_SET_DESKTOP_TITLE, (MPARAM) "New Desktop Title", NULL);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinSendMsg (hwndDocument, MDI_MSG_SET_INSTANCE_TITLE, (MPARAM) "New White Instance Title", NULL);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "White", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Respond to document notifications

         case WM_CONTROL:
            switch (HIUSHORT (mp1))
              {case MDI_INHIBIT_CLOSE_INSTANCE:
                  pv = (PVOID) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DESKTOP_DATA, (MPARAM) NULL, (MPARAM) NULL);
                  if (pv == NULL)
                     return (MRESULT) NULL;
                  pv = (PVOID) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);
                  if (pv == NULL)
                     return (MRESULT) NULL;
                  pv = (PVOID) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
                  if (pv == NULL)
                     return (MRESULT) NULL;
                  return (MRESULT) NULL;
               // return (MRESULT) (MBID_NO == WinMessageBox (HWND_DESKTOP, hwndDocument,
               //   "Do you really want to close?", "MDI White Document Close", 0, MB_ICONQUESTION | MB_YESNO));
               case MDI_INHIBIT_CLOSE_DESKTOP:
                  pv = (PVOID) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DESKTOP_DATA, (MPARAM) NULL, (MPARAM) NULL);
                  if (pv == NULL)
                     return (MRESULT) NULL;
                  pv = (PVOID) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);
                  if (pv == NULL)
                     return (MRESULT) NULL;
                  pv = (PVOID) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
                  if (pv == NULL)
                     return (MRESULT) NULL;
                  return (MRESULT) NULL;
               // return (MRESULT) (MBID_NO == WinMessageBox (HWND_DESKTOP, hwndDocument,
               //   "Do you really want to close?", "MDI White Desktop Close", 0, MB_ICONQUESTION | MB_YESNO));
               case MDI_NOTIFY_CASCADE_INSTANCE:
                  return (MRESULT) NULL;
               case MDI_NOTIFY_TILE_INSTANCE:
                  return (MRESULT) NULL;
               case MDI_NOTIFY_ACTIVATE_INSTANCE:
                  return (MRESULT) NULL;
               case MDI_NOTIFY_DEACTIVATE_INSTANCE:
                  return (MRESULT) NULL;
              }
            break;

      // Menu initialization

         case WM_INITMENU:
            fChecked = ! fChecked;
            WinSendMsg (HWNDFROMMP (mp2), MM_SETITEMATTR,
              MPFROM2SHORT (IDM_DOTHAT, TRUE),
              MPFROM2SHORT (MIA_CHECKED, (fChecked)? MIA_CHECKED : FALSE));
            fGrayed = ! fGrayed;
            WinSendMsg (HWNDFROMMP (mp2), MM_SETITEMATTR,
              MPFROM2SHORT (IDM_DOTHEOTHER, TRUE),
              MPFROM2SHORT (MIA_DISABLED, (fGrayed)? MIA_DISABLED : FALSE));
            return (MRESULT) NULL;

      // Menu termination

         case WM_MENUEND:
            return (MRESULT) NULL;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }


// Function: BlueWndProc -- "Blue" Document Window Procedure
// ---------------------------------------------------------
   MRESULT EXPENTRY BlueWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "Blue", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "Blue", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "Blue", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do This", "Blue", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do That", "Blue", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "Blue", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }


// Function: RedWndProc -- "Red" Document Window Procedure
// -------------------------------------------------------
   MRESULT EXPENTRY RedWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "Red", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "Red", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "Red", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do This", "Red", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do That", "Red", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "Red", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }


// Function: PinkWndProc -- "Pink" Document Window Procedure
// ---------------------------------------------------------
   MRESULT EXPENTRY PinkWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "Pink", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "Pink", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "Pink", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do This", "Pink", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do That", "Pink", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "Pink", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }


// Function: GreenWndProc -- "Green" Document Window Procedure
// -----------------------------------------------------------
   MRESULT EXPENTRY GreenWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "Green", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "Green", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "Green", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do This", "Green", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do That", "Green", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "Green", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }


// Function: CyanWndProc -- "Cyan" Document Window Procedure
// ---------------------------------------------------------
   MRESULT EXPENTRY CyanWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "Cyan", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "Cyan", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "Cyan", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do This", "Cyan", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do That", "Cyan", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "Cyan", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }


// Function: YellowWndProc -- "Yellow" Document Window Procedure
// -------------------------------------------------------------
   MRESULT EXPENTRY YellowWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Mesage parameter 1
      MPARAM mp2;                      // Mesage parameter 2

   // Define function data

     {HPS hps;
      RECTL rcl;
      COLOR clr;

   // Process PM messages

      switch (msg)

      // Process commands

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))
              {case IDM_FOO:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo", "Yellow", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_BAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Bar", "Yellow", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_FOOBAR:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Foo Bar", "Yellow", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHIS:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do This", "Yellow", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHAT:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do That", "Yellow", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
               case IDM_DOTHEOTHER:
                  WinMessageBox (HWND_DESKTOP, hwndDocument, "Do the Other", "Yellow", 0, MB_OK | MB_ICONEXCLAMATION);
                  return (MRESULT) NULL;
              }
            break;

      // Paint window

         case WM_PAINT:
            hps = WinBeginPaint (hwndDocument, (HPS) NULL, (PRECTL) NULL);
            clr = WinQueryWindowULong (hwndDocument, QWL_USER);
            WinQueryWindowRect (hwndDocument, &rcl);
            WinFillRect (hps, &rcl, clr);
            WinEndPaint (hps);
            return (MRESULT) NULL;
        }

   // Allow MDI to process all other messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2) ;

     }
