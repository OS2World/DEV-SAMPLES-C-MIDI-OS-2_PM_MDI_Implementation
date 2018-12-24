// Includes
// --------
   #define INCL_WIN
   #define INCL_GPICONTROL
   #define INCL_GPILCIDS
   #define INCL_GPIPRIMITIVES
   #define INCL_DOSFILEMGR

   #include <string.h>
   #include <stdlib.h>
   #include <os2.h>

   #include "Browser.H"
   #include "Browser.RH"

   #include "OpenDlg.H"
   #include "TellUser.H"
   #include "AllocMem.H"
   #include "MDI.H"


// Document Descriptor
// -------------------
   typedef struct _DOCUMENTDESC
     {USHORT offLine;                  // Line offset
      USHORT cchLine;                  // Line size
     } DOCUMENTDESC;

   typedef DOCUMENTDESC *PDOCUMENTDESC;


// Document Data
// -------------
   typedef struct _DOCUMENTDATA
     {BOOL fValid;                     // Is data valid?
      ULONG cLines;                    // Number of lines in document
      USHORT cchMax;                   // Largest line in document
      PDOCUMENTDESC pdesc;             // -->document descriptor
      CHAR chText[1];                  // Actual text
     } DOCUMENTDATA;

   typedef DOCUMENTDATA *PDOCUMENTDATA;


// Instance Data
// -------------
   typedef struct _INSTANCEDATA
     {ULONG idesc;                     // Current index into doc descriptor
      USHORT ich;                      // Current index into each line
      HPS hps;                         // Presentation space
      FONTMETRICS fm;                  // Font metrics
      SWP swp;                         // Document position and size
      HWND hwndHScroll;                // Horizontal scroll bar
      HWND hwndVScroll;                // Vertical scroll bar
      SHORT sVLines;                   // Vertical size in lines
      SHORT sHChars;                   // Horizontal size in characters
     } INSTANCEDATA;

   typedef INSTANCEDATA *PINSTANCEDATA;


// Constants
// ---------
   #define MAX_STRING     128          // Maximum string size
   #define LCID_FIXED     128L         // Local fixed pitch font id

   const ULONG cbMaxFile =             // Maximum file size
     (65535 - (sizeof (DOCUMENTDATA) - 1));

   const ULONG cMaxLines =             // Maximum number of lines in file
     (65534 / sizeof (USHORT));


// Function Declarations
// ---------------------
   VOID    EXPENTRY MDIInitialize          (HAB);

   MRESULT EXPENTRY BrowserDesktopWndProc  (HWND, USHORT, MPARAM, MPARAM);
   MRESULT FAR      BrowserDocumentCreate  (HWND, PINSTANCEDATA);
   MRESULT FAR      BrowserDocumentDestroy (HWND, PDOCUMENTDATA);
   MRESULT FAR      BrowserDocumentHScroll (HWND, USHORT, SHORT);
   MRESULT FAR      BrowserDocumentOpen    (HWND);
   MRESULT FAR      BrowserDocumentPaint   (HWND);
   MRESULT FAR      BrowserDocumentSize    (HWND);
   MRESULT FAR      BrowserDocumentVScroll (HWND, USHORT, SHORT);
   MRESULT EXPENTRY BrowserDocumentWndProc (HWND, USHORT, MPARAM, MPARAM) ;
   MRESULT FAR      BrowserInstanceDestroy (HWND, PINSTANCEDATA);


// Function: main - Create Browser desktop
// ---------------------------------------
   VOID main (VOID)

   // Define function data

     {HAB hab;                         // Application anchor block
      HMQ hmq;                         // Message queue handle
      QMSG qmsg ;                      // Message queue entry
      HWND hwndFrame, hwndDesktop;     // MDI desktop frame, client

      ULONG flCreateFlags =            // Creation flags
        FCF_STANDARD | FCF_NOBYTEALIGN;

   // Initialize PM procesing

      hab = WinInitialize (0);
      hmq = WinCreateMsgQueue (hab, 0);

   // Initialize MDI processing

      MDIInitialize (hab);

   // Create the MDI desktop

      hwndFrame = MDICreateDesktop (BrowserDesktopWndProc,
        flCreateFlags, "", (HMODULE) NULL, BROWSER_RESOURCES,
        &hwndDesktop, 0, 1, "E~xit", "E~xit", "~Window", 0L);
      if ((hwndFrame == (HWND) NULL) || (hwndDesktop == (HWND) NULL)) return;

   // Process PM messages

      while (WinGetMsg (hab, &qmsg, (HWND) NULL, 0, 0))
         WinDispatchMsg (hab, &qmsg) ;

   // Terminate PM processing

      WinDestroyMsgQueue (hmq) ;
      WinTerminate (hab) ;

     }


// Function: BrowserDesktopWndProc - Desktop window procedure
// ----------------------------------------------------------
   MRESULT EXPENTRY BrowserDesktopWndProc (hwndDesktop, msg, mp1, mp2)

      HWND hwndDesktop;                // Browser desktop window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Message parameter 1
      MPARAM mp2;                      // Message parameter 2

   // Process PM messages

     {switch (msg)

      // Process menu command

        {case WM_COMMAND:
            switch (LOUSHORT (mp1))

            // Open a file

              {case CMD_OPEN:
                  return BrowserDocumentOpen (hwndDesktop);

            // Display "about" box

               case CMD_ABOUT:
                  WinDlgBox (HWND_DESKTOP, hwndDesktop, WinDefDlgProc,
                    (HMODULE) NULL, DIALOG_ABOUT, (PVOID) NULL);
                  return (MRESULT) NULL;

            // End processing menu commands

              }

            break;

      // End processing PM messages

        }

   // Return to MDI

      return MDIDesktopWndProc (hwndDesktop, msg, mp1, mp2) ;

     }


// Function: BrowserDocumentCreate - Create document Window
// --------------------------------------------------------
   MRESULT FAR BrowserDocumentCreate (hwndDocument, pinst)

      HWND hwndDocument;               // Browser document window
      PINSTANCEDATA pinst;             // -->instance data

   // Define function data

     {HDC hdc;                         // Device context
      SIZEL sizl;                      // Presentation space size
      FONTMETRICS afm[14];             // List of available fonts
      FATTRS fat;                      // Font attributes
      ULONG cfm;                       // Number of fonts actualy available
      ULONG ifm;                       // Index into available fonts
      PDOCUMENTDATA pdoc;              // -->document data

   // Get a presentation space

      hdc = WinOpenWindowDC (hwndDocument);
      sizl.cx = 0;
      sizl.cy = 0;
      pinst->hps = GpiCreatePS ((HAB) NULL, hdc, &sizl, PU_PELS | GPIF_DEFAULT | GPIT_NORMAL | GPIA_ASSOC);

   // Create a fixed-pitch font for document painting. Note that the
   // Browser painting logic is optimized on the basis of the fixed
   // pitch font.

      cfm = sizeof (afm) / sizeof (FONTMETRICS);
      GpiQueryFonts (pinst->hps, QF_PUBLIC, "Courier", &cfm, (LONG)sizeof (FONTMETRICS), afm);
      for (ifm = 0; ifm < cfm; ifm++)
         if (!(afm[ifm].fsDefn & 0x0001)) break;
      fat.usRecordLength = sizeof(fat);
      fat.fsSelection = afm[ifm].fsSelection;
      fat.lMatch = afm[ifm].lMatch;
      strcpy (fat.szFacename, afm[ifm].szFacename);
      fat.idRegistry = afm[ifm].idRegistry;
      fat.usCodePage = 850;
      fat.lMaxBaselineExt = afm[ifm].lMaxBaselineExt;
      fat.lAveCharWidth = afm[ifm].lAveCharWidth;
      fat.fsType = 0;
      fat.fsFontUse = 0;
      GpiCreateLogFont (pinst->hps, (PSTR8) NULL, LCID_FIXED, &fat);
      GpiSetCharSet (pinst->hps, LCID_FIXED);

   // Store the font metrics for later use

      GpiQueryFontMetrics (pinst->hps, (LONG) sizeof (pinst->fm), &(pinst->fm));

   // Determine initial window position and size

      WinQueryWindowPos (hwndDocument, &(pinst->swp));

   // Locate document data

      pdoc = (PDOCUMENTDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);

   // Locate scroll bars

      pinst->hwndHScroll = WinWindowFromID (WinQueryWindow (hwndDocument,
        QW_PARENT, FALSE), FID_HORZSCROLL);
      pinst->hwndVScroll = WinWindowFromID (WinQueryWindow (hwndDocument,
        QW_PARENT, FALSE), FID_VERTSCROLL);

   // Indicate create handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentDestroy - Destroy Document
// ---------------------------------------------------
   MRESULT FAR BrowserDocumentDestroy (hwndDocument, pdoc)

      HWND hwndDocument;               // Browser document window
      PDOCUMENTDATA pdoc;              // -->document data

   // Fool the compiler

     {hwndDocument = hwndDocument;

   // Free document descriptor

      AllocMemFree (pdoc->pdesc);

   // Indicate destroy handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentHScroll - Horizontally Scroll Document
// ---------------------------------------------------------------
   MRESULT FAR BrowserDocumentHScroll (hwndDocument, usCmd, sPos)

      HWND hwndDocument;               // Browser document window
      USHORT usCmd;                    // Scrolling comand
      SHORT sPos;                      // Scrolling position

   // Define function data

     {SHORT sInc;                      // Scrolling increment
      PINSTANCEDATA pinst;             // -->instance data
      PDOCUMENTDATA pdoc;              // -->document data

   // Locate document and instance data

      pinst = (PINSTANCEDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
      pdoc = (PDOCUMENTDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);

   // Analyze scrolling command and determine scrolling increment

      switch (usCmd)
        {case SB_LINELEFT:    sInc = -1;
            break;
         case SB_LINERIGHT:   sInc = 1;
            break;
         case SB_PAGEUP:      sInc = min (-1, -pinst->sHChars);
            break;
         case SB_PAGEDOWN:    sInc = max (1, pinst->sHChars);
            break;
         case SB_SLIDERTRACK: sInc = sPos - (SHORT) pinst->ich;
            break;
         default:             sInc = 0;
            break;
        }
      sInc = max ((SHORT) pinst->ich * -1,
        min (sInc, (SHORT) (pdoc->cchMax - pinst->ich - pinst->sHChars)));

   // If necessary, perform actual scroll

      if (sInc != 0)
        {pinst->ich += sInc;
         WinScrollWindow (hwndDocument, -sInc * (SHORT) pinst->fm.lAveCharWidth, 0,
           (PRECTL) NULL, (PRECTL) NULL, (HRGN) NULL, (PRECTL) NULL, SW_INVALIDATERGN);
         WinSendMsg (pinst->hwndHScroll, SBM_SETPOS, MPFROMSHORT ((USHORT) pinst->ich), (MPARAM) NULL);
         WinUpdateWindow (hwndDocument);
        }

   // Indicate scroll handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentOpen - Open a new document
// ---------------------------------------------------
   MRESULT FAR BrowserDocumentOpen (hwndDesktop)

      HWND hwndDesktop;                // Browser desktop window

   // Define function data

     {HWND hwndFrame, hwndDocument;    // MDI document frame, client
      DLF dlf;                         // Open dialog interchange data
      HFILE hfile;                     // File handle
      FILESTATUS fsts;                 // File status
      CHAR szOpenTitle[MAX_STRING];    // Open dialog box title
      CHAR szOpenHelp[MAX_STRING];     // Open dialog box help
      PINSTANCEDATA pinst;             // -->instance data
      PDOCUMENTDATA pdoc;              // -->file document data
      USHORT cbBytes;                  // Number of bytes read
      USHORT ichText;                  // Index into document text
      USHORT offLine;                  // Line offset
      ULONG idesc;                     // Index into document descriptor

      ULONG flCreateFlags =            // Creation flags
        FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER | FCF_MINMAX |
        FCF_ICON | FCF_SHELLPOSITION | FCF_NOBYTEALIGN |
        FCF_VERTSCROLL | FCF_HORZSCROLL;

   // Obtain document name and open document, verifying document is not
   // too large to browse

      WinLoadString ((HAB) NULL, (HMODULE) NULL, STRING_OPENTITLE, sizeof (szOpenTitle), szOpenTitle);
      WinLoadString ((HAB) NULL, (HMODULE) NULL, STRING_OPENHELP, sizeof (szOpenHelp), szOpenHelp);
      while (TRUE)
        {SetupDLF (&dlf, DLG_OPENDLG, &hfile, "\\*.*", NULL, szOpenTitle, szOpenHelp);
         if (DlgFile (hwndDesktop, &dlf) != TDF_OLDOPEN) return;
         DosQFileInfo (hfile, 0x0001, (PBYTE) &fsts, sizeof (fsts));
         if (fsts.cbFile > cbMaxFile)
            TellUser (STRING_TOOLARGE, BROWSER_MODNAME, MB_OK, dlf.szFileName, fsts.cbFile, cbMaxFile);
         else break;
        }

   // Create document window

      hwndFrame = MDICreateDocument (BrowserDocumentWndProc, hwndDesktop,
        flCreateFlags, dlf.szFileName, (HMODULE) NULL, DOCUMENT_RESOURCES,
        &hwndDocument, (USHORT) (fsts.cbFile + (sizeof (DOCUMENTDATA) - 1)),
        sizeof (INSTANCEDATA), 1);
      if ((hwndFrame == (HWND) NULL) || (hwndDocument == (HWND) NULL)) return;

   // Locate document and instance data

      pinst = (PINSTANCEDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
      pdoc = (PDOCUMENTDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);

   // If document data is not yet valid...

      if (! pdoc->fValid)

      // Read entire file into document data and check that read was
      // successful

        {if (DosRead (hfile, pdoc->chText, (USHORT) fsts.cbFile, &cbBytes)
          || (fsts.cbFile != cbBytes))
           {TellUser (STRING_BADREAD, BROWSER_MODNAME, MB_OK, dlf.szFileName);
            return;
           }

      // Count the number of lines in the document and verify that
      // it is less than the maximum allowed

         for (ichText = 0; ichText < cbBytes; ichText++)
            if (pdoc->chText[ichText] == '\n') pdoc->cLines++;
         if (pdoc->cLines > cMaxLines)
           {TellUser (STRING_TOOMANY, BROWSER_MODNAME, MB_OK, dlf.szFileName, pdoc->cLines, cMaxLines);
            return;
           }

      // Allocate and fill the document descriptor array

         pdoc->pdesc = (PDOCUMENTDESC) AllocMemAlloc (pdoc->cLines * sizeof (DOCUMENTDESC));
         pdoc->cchMax = 0;
         offLine = 0;
         idesc = 0;
         for (ichText = 0; ichText < cbBytes; ichText++)
           {if (pdoc->chText[ichText] == '\r')
               pdoc->chText[ichText] = ' ';
            else if (pdoc->chText[ichText] == '\n')
              {pdoc->pdesc[idesc].offLine = offLine;
               pdoc->pdesc[idesc].cchLine = ichText - offLine;
               if (pdoc->pdesc[idesc].cchLine > pdoc->cchMax)
                  pdoc->cchMax = pdoc->pdesc[idesc].cchLine;
               offLine = ichText + 1;
               idesc++;
              }
           }

      // Validate document data

         pdoc->fValid = TRUE;
        }

   // Force document to be resized

      BrowserDocumentSize (hwndDocument);

   // Force document window to be re-painted

      WinInvalidateRect (hwndDocument, (PRECTL) NULL, FALSE);

   // Indicate open handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentPaint - Paint document
// -----------------------------------------------
   MRESULT FAR BrowserDocumentPaint (hwndDocument)

      HWND hwndDocument;               // Browser document window

   // Define function data

     {HPS hps;                         // Presentation space
      PINSTANCEDATA pinst;             // -->instance data
      PDOCUMENTDATA pdoc;              // -->document data
      RECTL rclInvalid;                // Invalid rectangle
      RECTL rclWnd;                    // Window rectangle
      RECTL rclLine;                   // Line rectangle
      ULONG cLines;                    // Count of lines in window
      ULONG idesc;                     // Index into document descriptor
      SHORT sTop;                      // Top line displacement
      SHORT sLeft;                     // Left character displacement
      POINTL ptl;                      // Temporary point

      static CHAR chDummy = ' ';       // Dummy line

   // Locate document and instance data

      pinst = (PINSTANCEDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
      pdoc = (PDOCUMENTDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);

   // Initialize painting

      WinQueryWindowRect (hwndDocument, &rclWnd);
      hps = WinBeginPaint (hwndDocument, pinst->hps, &rclInvalid);

   // Optimize by initially calculating the top line and left character
   // displacement using the invalid rectangle. Adjust the invalid rectangle
   // such that it starts at an integral line/character position. This
   // logic only works because we use a fixed pitch font.

      sTop = (SHORT) ((rclWnd.yTop - rclInvalid.yTop) / pinst->fm.lMaxBaselineExt);
      sLeft = (SHORT) ((rclInvalid.xLeft - rclWnd.xLeft) / pinst->fm.lAveCharWidth);
      rclInvalid.yTop = rclWnd.yTop - ((LONG) sTop * pinst->fm.lMaxBaselineExt);
      rclInvalid.xLeft = rclWnd.xLeft + ((LONG) sLeft * pinst->fm.lAveCharWidth);
      cLines = ((rclInvalid.yTop - rclInvalid.yBottom) / pinst->fm.lMaxBaselineExt) + 1L;

   // Initialize the rectangle that bounds each line to be painted

      rclLine.xLeft = rclInvalid.xLeft;
      rclLine.xRight = rclInvalid.xRight;
      rclLine.yTop = rclInvalid.yTop - 1L;

   // Draw each line

      for (idesc = pinst->idesc + sTop; idesc < pdoc->cLines; idesc++)
        {if (cLines-- == 0) break;
         rclLine.yBottom = rclLine.yTop - pinst->fm.lMaxBaselineExt + 1L;
         ptl.x = rclLine.xLeft;
         ptl.y = rclLine.yBottom + pinst->fm.lMaxDescender;
         if (pdoc->pdesc[idesc].cchLine > (USHORT) (sLeft + pinst->ich))
           {GpiCharStringPosAt (hps, &ptl, &rclLine, CHS_OPAQUE,
              (LONG) (pdoc->pdesc[idesc].cchLine - sLeft - pinst->ich),
              pdoc->chText + pdoc->pdesc[idesc].offLine + sLeft + pinst->ich, (PLONG) NULL);
           }
         else GpiCharStringPosAt (hps, &ptl, &rclLine, CHS_OPAQUE, 1L, &chDummy, (PLONG) NULL);
         rclLine.yTop = rclLine.yBottom - 1L;
        }

   // Fill the remainder (if any) of the window

      rclLine.yTop++;
      rclLine.yBottom = rclInvalid.yBottom;
      WinFillRect (hps, &rclLine, CLR_WHITE);

   // Terminate painting

      WinEndPaint (hps);

   // Indicate paint handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentSize - Resize document
// -----------------------------------------------
   MRESULT FAR BrowserDocumentSize (hwndDocument)

      HWND hwndDocument;               // Browser document window

   // Define function data

     {PINSTANCEDATA pinst;             // -->instance data
      PDOCUMENTDATA pdoc;              // -->document data

   // Locate document and instance data

      pinst = (PINSTANCEDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
      pdoc = (PDOCUMENTDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);

   // Determine new window position and size

      WinQueryWindowPos (hwndDocument, &(pinst->swp));

   // Determine new dimensions in lines and characters

      pinst->sVLines = pinst->swp.cy / (SHORT) pinst->fm.lMaxBaselineExt;
      pinst->sHChars = pinst->swp.cx / (SHORT) pinst->fm.lAveCharWidth;

   // Set scroll bar thumb sizes

      WinSendMsg (pinst->hwndHScroll, SBM_SETTHUMBSIZE,
        MPFROM2SHORT (pinst->sHChars, pdoc->cchMax), (MPARAM) NULL);
      WinSendMsg (pinst->hwndVScroll, SBM_SETTHUMBSIZE,
        MPFROM2SHORT (pinst->sVLines, pdoc->cLines), (MPARAM) NULL);

   // Set scroll bar limits

      WinSendMsg (pinst->hwndHScroll, SBM_SETSCROLLBAR,
        MPFROMSHORT (pinst->ich), MPFROM2SHORT (0, (USHORT) pdoc->cchMax - pinst->sHChars-1));
      WinSendMsg (pinst->hwndVScroll, SBM_SETSCROLLBAR,
        MPFROMSHORT (pinst->idesc), MPFROM2SHORT (0, (USHORT) pdoc->cLines- pinst->sVLines-1));

   // Indicate resize handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentVScroll - Vertically Scroll Document
// -------------------------------------------------------------
   MRESULT FAR BrowserDocumentVScroll (hwndDocument, usCmd, sPos)

      HWND hwndDocument;               // Browser document window
      USHORT usCmd;                    // Scrolling comand
      SHORT sPos;                      // Scrolling position

   // Define function data

     {SHORT sInc;                      // Scrolling increment
      PINSTANCEDATA pinst;             // -->instance data
      PDOCUMENTDATA pdoc;              // -->document data

   // Locate document and instance data

      pinst = (PINSTANCEDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_INSTANCE_DATA, (MPARAM) NULL, (MPARAM) NULL);
      pdoc = (PDOCUMENTDATA) WinSendMsg (hwndDocument, MDI_MSG_LOCATE_DOCUMENT_DATA, (MPARAM) NULL, (MPARAM) NULL);

   // Analyze scrolling command and determine scrolling increment

      switch (usCmd)
        {case SB_LINEUP:      sInc = -1;
            break;
         case SB_LINEDOWN:    sInc = 1;
            break;
         case SB_PAGEUP:      sInc = min (-1, -pinst->sVLines);
            break;
         case SB_PAGEDOWN:    sInc = max (1, pinst->sVLines);
            break;
         case SB_SLIDERTRACK: sInc = sPos - (SHORT) pinst->idesc;
            break;
         default:             sInc = 0;
            break;
        }
      sInc = max ((SHORT) pinst->idesc * -1,
        min (sInc, (SHORT) (pdoc->cLines - pinst->idesc - pinst->sVLines)));

   // If necessary, perform actual scroll

      if (sInc != 0)
        {pinst->idesc += sInc;
         WinScrollWindow (hwndDocument, 0, sInc * (SHORT) pinst->fm.lMaxBaselineExt,
           (PRECTL) NULL, (PRECTL) NULL, (HRGN) NULL, (PRECTL) NULL, SW_INVALIDATERGN);
         WinSendMsg (pinst->hwndVScroll, SBM_SETPOS, MPFROMSHORT ((USHORT) pinst->idesc), (MPARAM) NULL);
         WinUpdateWindow (hwndDocument);
        }

   // Indicate scroll handled

      return (MRESULT) NULL;

     }


// Function: BrowserDocumentWndProc - Document window procedure
// ------------------------------------------------------------
   MRESULT EXPENTRY BrowserDocumentWndProc (hwndDocument, msg, mp1, mp2)

      HWND hwndDocument;               // Browser document window
      USHORT msg;                      // PM message
      MPARAM mp1;                      // Message parameter 1
      MPARAM mp2;                      // Message parameter 2

   // Define function data

     {USHORT fsKeyFlags;               // From WM_CHAR
      UCHAR uchRepeat;                 // From WM_CHAR
      USHORT usVKey;                   // From WM_CHAR

   // Process PM messages

      switch (msg)

      // MDI control messages

        {case WM_CONTROL:
            switch (SHORT2FROMMP (mp1))

            // Process destruction of document

              {case MDI_NOTIFY_CREATE_INSTANCE:
                  return BrowserDocumentCreate (hwndDocument, (PINSTANCEDATA) mp2);

            // Process destruction of document

               case MDI_NOTIFY_DESTROY_DOCUMENT:
                  return BrowserDocumentDestroy (hwndDocument, (PDOCUMENTDATA) mp2);

            // Process destruction of instance

               case MDI_NOTIFY_DESTROY_INSTANCE:
                  return BrowserInstanceDestroy (hwndDocument, (PINSTANCEDATA) mp2);
              }
            return (MRESULT) NULL;

      // Simulate scrolling from character keys

         case WM_CHAR:
            fsKeyFlags = SHORT1FROMMP (mp1);
            uchRepeat = CHAR3FROMMP (mp1);
            usVKey = SHORT2FROMMP (mp2);
            if (! (fsKeyFlags & KC_KEYUP) && (fsKeyFlags & KC_VIRTUALKEY))
              {while (uchRepeat--)
                 {switch (usVKey)
                    {case VK_LEFT:
                       BrowserDocumentHScroll (hwndDocument, SB_LINELEFT, 0);
                       break;
                     case VK_RIGHT:
                       BrowserDocumentHScroll (hwndDocument, SB_LINERIGHT, 0);
                       break;
                     case VK_UP:
                       BrowserDocumentVScroll (hwndDocument, SB_LINEUP, 0);
                       break;
                     case VK_DOWN:
                       BrowserDocumentVScroll (hwndDocument, SB_LINEDOWN, 0);
                       break;
                     case VK_PAGEUP:
                       BrowserDocumentVScroll (hwndDocument, SB_PAGEUP, 0);
                       break;
                     case VK_PAGEDOWN:
                       BrowserDocumentVScroll (hwndDocument, SB_PAGEDOWN, 0);
                       break;
                    }
                 }
              }
            return (MRESULT) NULL;

      // Horizontally scroll document

         case WM_HSCROLL:
            return BrowserDocumentHScroll (hwndDocument, SHORT2FROMMP (mp2), SHORT1FROMMP (mp2));

      // Process paint request for document

         case WM_PAINT:
            return BrowserDocumentPaint (hwndDocument);

      // Process resize request for document

         case WM_SIZE:
            return BrowserDocumentSize (hwndDocument);

      // Vertically scroll document

         case WM_VSCROLL:
            return BrowserDocumentVScroll (hwndDocument, SHORT2FROMMP (mp2), SHORT1FROMMP (mp2));

      // End processing PM messages

        }

   // Allow MDI to process all messages

      return MDIDocumentWndProc (hwndDocument, msg, mp1, mp2);

     }


// Function: BrowserInstanceDestroy - Destroy Document Window
// ----------------------------------------------------------
   MRESULT FAR BrowserInstanceDestroy (hwndDocument, pinst)

      HWND hwndDocument;               // Browser document window
      PINSTANCEDATA pinst;             // -->instance data

   // Fool the compiler

     {hwndDocument = hwndDocument;

   // Destroy font and presentation space

      GpiSetCharSet (pinst->hps, (LONG) NULL);
      GpiDeleteSetId (pinst->hps, LCID_FIXED);
      GpiDestroyPS (pinst->hps);

   // Indicate destroy handled

      return (MRESULT) NULL;

     }
