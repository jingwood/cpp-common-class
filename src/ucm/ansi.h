///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef ansi_h
#define ansi_h

#define ANSI_ESC "\33"
#define ANSI_CSI ANSI_ESC"["

#define ANSI_CLN_LEFT 		ANSI_CSI"0K"		// clear line from cursor right
#define ANSI_CLN_RIGHT 		ANSI_CSI"1K"		// clear line from cursor left
#define ANSI_CLN_ENTIRE 	ANSI_CSI"2K"		// clear entire line

#define ANSI_COL_START		ANSI_CSI"1A"		// move to line start
#define ANSI_RESET_LINE		ANSI_CLN_ENTIRE "" ANSI_CLN_LEFT"\r"

#define ANSI_CSI_FBLK "30;"
#define ANSI_CSI_FRED "31;"
#define ANSI_CSI_FGRN "32;"
#define ANSI_CSI_FYEL "33;"
#define ANSI_CSI_FBLU "34;"
#define ANSI_CSI_FMAG "35;"
#define ANSI_CSI_FCYN "36;"
#define ANSI_CSI_FWHT "37;"

#define ANSI_CSI_BBLK "40;"
#define ANSI_CSI_BRED "41;"
#define ANSI_CSI_BGRN "42;"
#define ANSI_CSI_BYEL "43;"
#define ANSI_CSI_BBLU "44;"
#define ANSI_CSI_BMAG "45;"
#define ANSI_CSI_BCYN "46;"
#define ANSI_CSI_BWHT "47;"

#define ANSI_NOR ANSI_CSI"0m"
#define ANSI_BOLD ANSI_CSI"1;1m"

#define ANSI_FBLK ANSI_CSI ANSI_CSI_FBLK"1m"
#define ANSI_FRED ANSI_CSI ANSI_CSI_FRED"1m"
#define ANSI_FGRN ANSI_CSI ANSI_CSI_FGRN"1m"
#define ANSI_FYEL ANSI_CSI ANSI_CSI_FYEL"1m"
#define ANSI_FBLU ANSI_CSI ANSI_CSI_FBLU"1m"
#define ANSI_FMAG ANSI_CSI ANSI_CSI_FMAG"1m"
#define ANSI_FCYN ANSI_CSI ANSI_CSI_FCYN"1m"
#define ANSI_FWHT ANSI_CSI ANSI_CSI_FWHT"1m"

#define ANSI_BBLK ANSI_CSI ANSI_CSI_BBLK"1m"
#define ANSI_BRED ANSI_CSI ANSI_CSI_BRED"1m"
#define ANSI_BGRN ANSI_CSI ANSI_CSI_BGRN"1m"
#define ANSI_BYEL ANSI_CSI ANSI_CSI_BYEL"1m"
#define ANSI_BBLU ANSI_CSI ANSI_CSI_BBLU"1m"
#define ANSI_BMAG ANSI_CSI ANSI_CSI_BMAG"1m"
#define ANSI_BCYN ANSI_CSI ANSI_CSI_BCYN"1m"
#define ANSI_BWHT ANSI_CSI ANSI_CSI_BWHT"1m"

#endif /* ansi_h */
