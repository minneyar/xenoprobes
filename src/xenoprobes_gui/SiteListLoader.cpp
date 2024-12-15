/**
 * @file SiteListLoader.cpp
 *
 * @author Dan Keenan
 * @date 11/23/24
 * @copyright GNU GPLv3
 */

#include "SiteListLoader.h"
#include <QFile>
#include <QJsonArray>
#include <QTextStream>
#include <ranges>

std::unordered_set<Site::Id>
SiteListLoader::readSiteListFromFile(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Failed to open file.");
  }
  QTextStream in(&file);
  std::unordered_set<Site::Id> ids;
  QString line;
  bool ok;
  while (in.readLineInto(&line)) {
    line = line.trimmed();
    if (line.isEmpty() || line.startsWith('#')) {
      // Comment or blank line.
      continue;
    }
    const auto idStr = line.first(line.indexOf(','));
    const auto id = idStr.toUInt(&ok, 10);
    // Some sanity checking on the file.
    if (!ok) {
      throw std::runtime_error("Failed to parse id.");
    }
    if (!Site::ALL.contains(id)) {
      throw std::runtime_error("Site not found.");
    }

    ids.insert(id);
  }

  return ids;
}

std::unordered_set<Site::Id>
SiteListLoader::readSiteListFromJson(const QJsonValue &json) {
  if (!json.isArray()) {
    throw std::runtime_error("Bad site list format.");
  }
  std::unordered_set<Site::Id> ids;
  for (const auto id : json.toArray()) {
    if (!id.isDouble()) {
      throw std::runtime_error("Failed to parse id.");
    }
    if (!Site::ALL.contains(id.toInt())) {
      throw std::runtime_error("Site not found.");
    }
    ids.insert(id.toInt());
  }

  return ids;
}

void SiteListLoader::writeSiteListToFile(
    const std::unordered_set<Site::Id> &ids, const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate |
                 QIODevice::Text)) {
    throw std::runtime_error("Failed to open file.");
  }

  // Write header.
  QTextStream out(&file);
  out << "# Format:\n"
         "#   SiteName,MiningGrade,RevenueGrade,CombatGrade,SightseeingSpots,"
         "RareOre,RareOre,RareOre,...\n"
         "#\n"
         "# Everything after a # will be ignored.\n";
  // Write all sites, commenting out those that are not in ids.
  for (const auto &site : Site::ALL | std::views::values) {
    if (!ids.contains(site.name)) {
      out << '#';
    }
    out << site.name << ',' << Site::gradeToChar(site.production) << ','
        << Site::gradeToChar(site.revenue) << ','
        << Site::gradeToChar(site.combat) << ',' << site.sightseeing;
    for (const auto &ore : site.getOre()) {
      out << ',' << QString::fromStdString(ore->name);
    }
    out << '\n';
  }
  out.flush();
}

QJsonValue
SiteListLoader::writeSiteListToJson(const std::unordered_set<Site::Id> &ids) {
  QJsonArray json;
  for (const auto id : ids) {
    json.append(static_cast<int>(id));
  }
  return json;
}
