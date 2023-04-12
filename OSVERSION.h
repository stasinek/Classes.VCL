#define _WIN32_WIN95 0x0400 // Windows 95
#define _WIN32_WIN98 0x0410 // Windows 98
#define _WIN32_WINME 0x0490 // Windows ME
#define _WIN32_WINNT 0x0400 // NT 4.0 (_WIN32_WINNT defined)
#define _WIN32_WIN2K 0x0500 // Windows 2000
#define _WIN32_WINXP 0x0501 // Windows XP
#define _WIN32_WIN10 0x0A00 // Windows 10

#define _WIN32_WINNT_NT4                    0x0400 // Windows NT 4.0
#define _WIN32_WINNT_WIN2K                  0x0500 // Windows 2000
#define _WIN32_WINNT_WINXP                  0x0501 // Windows XP
#define _WIN32_WINNT_WS03                   0x0502 // Windows Server 2003
#define _WIN32_WINNT_WIN6                   0x0600 // Windows Vista
#define _WIN32_WINNT_VISTA                  0x0600 // Windows Vista
#define _WIN32_WINNT_WS08                   0x0600 // Windows Server 2008
#define _WIN32_WINNT_LONGHORN               0x0600 // Windows Vista
#define _WIN32_WINNT_WIN7                   0x0601 // Windows 7
#define _WIN32_WINNT_WIN8                   0x0602 // Windows 8
#define _WIN32_WINNT_WINBLUE                0x0603 // Windows 8.1
#define _WIN32_WINNT_WINTHRESHOLD           0x0A00 // Windows 10
#define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10

#define OSVERSION_MASK      0xFFFF0000
#define SPVERSION_MASK      0x0000FF00

#define OSVER(Version)  (((Version) & OSVERSION_MASK))
#define SPVER(Version)  (((Version) & SPVERSION_MASK) >> 8)