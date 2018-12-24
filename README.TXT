San Francisco Canyon Comapny
220 Montgomery Street, Suite 1063
San Francisco, CA 94104

(415) 398-9957  -  FAX 398-5998

Compuserve 72371,104

Mark Florence, President



MDI DLL
-------
You have just downloaded a set of tools that make writing MDI applications
under OS/2 PM extemely simple. MDI.DLL does all the work (a couple of
small "helper" DLLs assist). MDI.H, which must be included in all
programs that use MDI, defines the API. Two sample programs, one very
basic and one more useful, illustrate how to use the small number
of messages and functions defined.

The MDI DLL was developed by the San Francisco Canyon Company for
internal use. If you like what you see and you find it useful, please
contact Canyon and we'll send you printed documentation
(only the most basic documentation is included here), a
QuickHelp database for the MDI messages and function calls and we'll
keep you up-to-date with fixes and changes. Of course, any suggestions
or problem reports will be gratefully received.

Good luck!

Fixes and Enhancements
----------------------
See the enclosed READ.nn files (where "nn" is the MDI release level).

Highlights
----------

  * details of the MDI user interface are completely handled

  * a "desktop" window maintains the basic MDI interface

  * multiple "document" windows can be opened within the "desktop"

  * the title of each "document" is automatically maintained.
    Documents of the same name are automatically assigned a ":n"
    suffix.

  * a "window" menu entry is automatically created and maintained
    within the "desktop" menu bar

  * the system menu for "document" windows is automatically altered
    for the MDI accelerator conventions

  * the positions of the "desktop" and "document" windows are
    automatically remembered by name on OS2.INI

  * a re-entrant data area can be automatically created for the
    "desktop" window

  * a re-entrant data area (document instance data) can be automatically
    created for each "document" window. This can be used, for example,
    to maintain positioning within a document.

  * a re-entrant data area (document data) can be automatically created
    for each like-named document (ignoring any ":n" sufix). This can be
    used, for example, to maintain the contents of the document.

  * simple messages return the locations of the re-entrant data areas

Restrictions
------------
Note: refer to the sample programs (TEST.C and BROWSER.C) for
clarification of these points.

  * your desktop and document window procedures (as named in the
    MDICreateDesktop and MDICreateDocument calls) cannot process
    the WM_CREATE or WM_DESTROY messages. Instead, they can process
    the MDI_NOTIFY_* messages documented in MDI.H.

  * all messages must be passed to MDIDesktopWndProc or
    MDIDocumentWndProc as appropriate. This includes WM_COMMAND
    or WM_CONTROL messages

  * currently, MDI.DLL reserves the values 32000-32767 as the command
    value in WM_COMMAND messages. This means, for example, that you
    can't use this range as menu ids. This restriction is expected
    to be removed in a future release.

MDI-nn.ARC Contents
----------------
MDI-nn.ARC contains the source for these components:

  TELLUSER.DLL : Internal user notification DLL
  ALLOCMEM.DLL : Internal memory management DLL
  MDI.DLL      : MDI User Interface DLL
  TEST.EXE     : Basic MDI test program
  BROWSER.EXE  : Basic MDI file Browser

In addition, these files are provided to help you get started immediately
with the MDI interface. You may already have them installed on your system.

  OPENDLG.DLL  : Open Dialog Box DLL (from Tool Kit samples --
                 used only by the file Browser)

Installation
------------
To install MDI, compile the source code as described below. Make sure
that you put the DLLs (MDI.DLL, ALLOCMEM.DLL and TELLUSER.DLL) in a
directory referenced by the LIBPATH= CONFIG.SYS parameter.
Execute TEST to see a basic demonstration of the capabilities
of the MDI interface. Execute BROWSER to see a more comprehensive (and
useful!) example.

Compilation
-----------
To recompile the MDI interface, use the following model statements
and adapt them to suit your configuration. You will need to change
the compiler and linker options to your installation standards. We
recommend using the multithread options and the C-run time DLL, as
described in Chapter 14 of the MSC 6.0 Advanced Programming Techniques
Manual.

The example shown below is written for MSC 5.1.

 cl /Alfw /c /DDLL /FPa /Gs2w /Od /W3 /Zelp TellUser.C
 link /M /NOD /NOI /CO TellUser+CRTDLL, TellUser.DLL, TellUser.MAP, CRTLIB OS2, TellUser.DEF;
 implib TellUser.LIB TellUser.DEF
 cl /Alfw /c /DDLL /FPa /Gs2w /Od /W3 /Zelp AllocMem.C
 rc -r AllocMem.RC
 link /M /NOD /NOI /CO AllocMem+CRTDLL, AllocMem.DLL, AllocMem.MAP, TellUser CRTLIB OS2, AllocMem.def;
 rc AllocMem.RES AllocMem.DLL
 implib ALLOCMEM.LIB AllocMem.def
 cl /Alfw /c /DDLL /FPa /Gs2w /Od /W3 /Zelp MDI.C
 rc -r MDI.RC
 link /M /NOD /NOI /CO MDI+CRTDLL, MDI.DLL, MDI.MAP, AllocMem TellUser CRTLIB OS2, MDI.DEF;
 rc MDI.RES MDI.DLL
 implib MDI.LIB MDI.DEF
 cl /AL /c /DDLL /FPa /Gs2w /Od /W3 /Zelp TEST.C
 rc -r TEST.RC
 link /M /NOD /NOI /CO TEST+CRTEXE, TEST.EXE, TEST.MAP, MDI CRTLIB OS2, TEST.DEF;
 rc TEST.RES TEST.EXE
 cl /AL /c /DDLL /FPa /Gs2w /Od /W3 /Zelp Browser.C
 rc -r Browser.RC
 link /M /NOD /NOI /CO Browser+CRTEXE, Browser.EXE, Browser.MAP, AllocMem TellUser OPENDLG MDI CRTLIB OS2, Browser.DEF;
 rc Browser.RES Browser.EXE

The example shown below is written for MSC 6.0.

 cl /c /Gs /MD /Od /W3 /Zelp TellUser.C
 link /M /NOD /NOI /CO TellUser+CRTDLL, TellUser.DLL, TellUser.MAP, MSC60LIB OS2, TellUser.DEF;
 implib TellUser.LIB TellUser.DEF
 cl /c /Gs /MD /Od /W3 /Zelp AllocMem.C
 rc -r AllocMem.RC
 link /M /NOD /NOI /CO AllocMem+CRTDLL, AllocMem.DLL, AllocMem.MAP, TellUser MSC60LIB OS2, AllocMem.def;
 rc AllocMem.RES AllocMem.DLL
 implib ALLOCMEM.LIB AllocMem.def
 cl /c /Gs /MD /Od /W3 /Zelp MDI.C
 rc -r MDI.RC
 link /M /NOD /NOI /CO MDI+CRTDLL, MDI.DLL, MDI.MAP, AllocMem TellUser MSC60LIB OS2, MDI.DEF;
 rc MDI.RES MDI.DLL
 implib MDI.LIB MDI.DEF
 cl /c /MD /Od /W3 /Zelp TEST.C
 rc -r TEST.RC
 link /M /NOD /NOI /CO TEST+CRTEXE, TEST.EXE, TEST.MAP, MDI MSC60LIB OS2, TEST.DEF;
 rc TEST.RES TEST.EXE
 cl /c /MD /Od /W3 /Zelp Browser.C
 rc -r Browser.RC
 link /M /NOD /NOI /CO Browser+CRTEXE, Browser.EXE, Browser.MAP, AllocMem TellUser OPENDLG MDI MSC60LIB OS2, Browser.DEF;
 rc Browser.RES Browser.EXE
