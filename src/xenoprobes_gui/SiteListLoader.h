/**
 * @file SiteListLoader.h
 *
 * @author Dan Keenan
 * @date 11/23/24
 * @copyright GNU GPLv3
 */

#ifndef SITELISTLOADER_H
#define SITELISTLOADER_H

#include <QString>
#include "FnSite.h"

/**
 * Read a site list.
 *
 * This uses the same format as the command-line xenoprobes, but ignores the extra metadata.
 */
class SiteListLoader {
public:
    static FnSite::IdList readSiteList(const QString& path);
    static void writeSiteList(const FnSite::IdList& ids, const QString& path);
};



#endif //SITELISTLOADER_H
