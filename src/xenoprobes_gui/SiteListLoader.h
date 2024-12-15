/**
 * @file SiteListLoader.h
 *
 * @author Dan Keenan
 * @date 11/23/24
 * @copyright GNU GPLv3
 */

#ifndef SITELISTLOADER_H
#define SITELISTLOADER_H

#include "FnSiteWidget.h"
#include <QJsonValue>
#include <unordered_set>

/**
 * Read a site list.
 *
 * This uses the same format as the command-line xenoprobes, but ignores the
 * extra metadata.
 */
class SiteListLoader {
public:
  [[nodiscard]] static std::unordered_set<Site::Id>
  readSiteListFromFile(const QString &path);
  [[nodiscard]] static std::unordered_set<Site::Id>
  readSiteListFromJson(const QJsonValue &json);
  static void writeSiteListToFile(const std::unordered_set<Site::Id> &ids,
                                  const QString &path);
  [[nodiscard]] static QJsonValue
  writeSiteListToJson(const std::unordered_set<Site::Id> &ids);
};

#endif // SITELISTLOADER_H
