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
#include <probeoptimizer/probe_optimizer.h>
#include <ranges>

ProbeOptimizer::SiteList
SiteListLoader::readSiteListFromFile(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Failed to open file.");
  }
  QTextStream in(&file);
  ProbeOptimizer::SiteList siteList;
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

    try {
      siteList.insert(Site::fromName(id));
    } catch (const std::exception &e) {
      throw std::runtime_error("Site not found.");
    }
  }

  return siteList;
}

ProbeOptimizer::SiteList
SiteListLoader::readSiteListFromJson(const QJsonValue &json) {
  if (!json.isArray()) {
    throw std::runtime_error("Bad site list format.");
  }
  ProbeOptimizer::SiteList siteList;
  for (const auto id : json.toArray()) {
    if (!id.isDouble()) {
      throw std::runtime_error("Failed to parse id.");
    }
    if (!Site::ALL.contains(id.toInt())) {
      throw std::runtime_error("Site not found.");
    }
    try {
      siteList.insert(Site::fromName(id.toInt()));
    } catch (const std::exception &e) {
      throw std::runtime_error("Site not found.");
    }
  }

  return siteList;
}

void SiteListLoader::writeSiteListToFile(
    const ProbeOptimizer::SiteList &siteList, const QString &path) {
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
  for (const auto site : Site::ALL | std::views::values) {
    if (!siteList.contains(site)) {
      out << '#';
    }
    out << site->name << ',' << Site::gradeToChar(site->production) << ','
        << Site::gradeToChar(site->revenue) << ','
        << Site::gradeToChar(site->combat) << ',' << site->sightseeing;
    for (const auto &ore : site->getOre()) {
      out << ',' << QString::fromStdString(ore->name);
    }
    out << '\n';
  }
  out.flush();
}

QJsonValue
SiteListLoader::writeSiteListToJson(const ProbeOptimizer::SiteList &siteList) {
  QJsonArray json;
  for (const auto site : siteList) {
    json.append(static_cast<int>(site->name));
  }
  return json;
}

QJsonValue SiteListLoader::writeSiteProbesToJson(
    const ProbeArrangement &probeArrangement) {
  // Do this as an array of 2-tuples so we don't have to worry about int parsing
  // when reading.
  QJsonArray json;
  for (const auto [site, probe] : probeArrangement.getSetup()) {
    json.append(QJsonArray(
        {static_cast<int>(site->name), QString::fromStdString(probe->id)}));
  }

  return json;
}

ProbeArrangement
SiteListLoader::readSiteProbesFromJson(const QJsonValue &json) {
  if (!json.isArray()) {
    throw std::runtime_error("Bad site probes map format.");
  }
  ProbeArrangement probeArrangement;
  probeArrangement.resize(json.toArray().size());
  for (const auto &siteProbe : json.toArray()) {
    if (!siteProbe.isArray() || siteProbe.toArray().size() != 2) {
      throw std::runtime_error("Bad site probe format.");
    }
    const auto siteProbeInfo = siteProbe.toArray();
    const auto siteId = siteProbeInfo[0];
    if (!siteId.isDouble()) {
      throw std::runtime_error("Bad site id.");
    }
    Site::Ptr site;
    try {
      site = Site::fromName(siteId.toInt());
    } catch (const std::out_of_range &) {
      throw std::runtime_error("Bad site id.");
    }
    const auto probeId = siteProbeInfo[1];
    if (!probeId.isString()) {
      throw std::runtime_error("Bad probe id.");
    }
    Probe::Ptr probe;
    try {
      probe = Probe::fromString(probeId.toString().toStdString());
    } catch (const std::out_of_range &) {
      throw std::runtime_error("Bad probe id.");
    }
    probeArrangement.setProbeAt(ProbeOptimizer::getIndexForSiteId(site->name),
                                probe);
  }
  return probeArrangement;
}
