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
#include <probeoptimizer/probe_arrangement.h>
#include <probeoptimizer/probe_optimizer.h>
#include <unordered_set>

/**
 * Read a site list.
 *
 * This uses the same format as the command-line xenoprobes, but ignores the
 * extra metadata.
 */
class SiteListLoader {
public:
  [[nodiscard]] static ProbeOptimizer::SiteList
  readSiteListFromFile(const QString &path);
  [[nodiscard]] static ProbeOptimizer::SiteList
  readSiteListFromJson(const QJsonValue &json);

  static void writeSiteListToFile(const ProbeOptimizer::SiteList &siteList,
                                  const QString &path);
  [[nodiscard]] static QJsonValue
  writeSiteListToJson(const ProbeOptimizer::SiteList &siteList);

  [[nodiscard]] static QJsonValue
  writeSiteProbesToJson(const ProbeArrangement &probeArrangement);
  [[nodiscard]] static ProbeArrangement
  readSiteProbesFromJson(const QJsonValue &json);
};

#endif // SITELISTLOADER_H
