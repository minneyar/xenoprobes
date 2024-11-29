/**
 * @file SiteListLoader.h
 *
 * @author Dan Keenan
 * @date 11/23/24
 * @copyright GNU GPLv3
 */

#ifndef SITELISTLOADER_H
#define SITELISTLOADER_H

#include "FnSite.h"
#include <QJsonValue>

/**
 * Read a site list.
 *
 * This uses the same format as the command-line xenoprobes, but ignores the
 * extra metadata.
 */
class SiteListLoader {
public:
  [[nodiscard]] static FnSite::IdList readSiteListFromFile(const QString &path);
  [[nodiscard]] static FnSite::IdList readSiteListFromJson(const QJsonValue &json);
  static void writeSiteListToFile(const FnSite::IdList &ids,
                                  const QString &path);
  [[nodiscard]] static QJsonValue writeSiteListToJson(const FnSite::IdList &ids);
};

#endif // SITELISTLOADER_H
