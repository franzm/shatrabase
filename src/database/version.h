/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#define SBASE_VERSION_MAJOR   0
#define SBASE_VERSION_MINOR   9
#define SBASE_REVISION        7

#define SBASE_STRINGIFYX(a__) #a__
#define SBASE_STRINGIFY(a__) SBASE_STRINGIFYX(a__)

#define SBASE_STR_VERSION     SBASE_STRINGIFY(SBASE_VERSION_MAJOR) "." \
                              SBASE_STRINGIFY(SBASE_VERSION_MINOR)
#define SBASE_STR_REVISION    SBASE_STRINGIFY(SBASE_VERSION_MAJOR) "." \
                              SBASE_STRINGIFY(SBASE_VERSION_MINOR) "." \
                              SBASE_STRINGIFY(SBASE_REVISION)

#define SBASE_COPYRIGHT "Copyright (C) 2016"
