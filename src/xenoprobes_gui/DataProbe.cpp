/**
 * @file DataProbe.cpp
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#include "DataProbe.h"
#include <QApplication>

QString dataProbeIcon(Probe::Ptr probe) {
  switch (probe->category) {
  case Probe::Category::Basic:
    return QStringLiteral(":/probe_icons/basic.png");
  case Probe::Category::Mining:
    return QStringLiteral(":/probe_icons/mining.png");
  case Probe::Category::Research:
    return QStringLiteral(":/probe_icons/research.png");
  case Probe::Category::Booster:
    return QStringLiteral(":/probe_icons/booster.png");
  case Probe::Category::Storage:
    return QStringLiteral(":/probe_icons/storage.png");
  case Probe::Category::Duplicator:
    return QStringLiteral(":/probe_icons/duplicator.png");
  case Probe::Category::Battle:
    return QStringLiteral(":/probe_icons/battle.png");
  }
  Q_UNREACHABLE();
}

static const QHash<Probe::Id, QString> kProbeNames{
    {"M1", qApp->translate("DataProbe", "Mining Probe G1")},
    {"M2", qApp->translate("DataProbe", "Mining Probe G2")},
    {"M3", qApp->translate("DataProbe", "Mining Probe G3")},
    {"M4", qApp->translate("DataProbe", "Mining Probe G4")},
    {"M5", qApp->translate("DataProbe", "Mining Probe G5")},
    {"M6", qApp->translate("DataProbe", "Mining Probe G6")},
    {"M7", qApp->translate("DataProbe", "Mining Probe G7")},
    {"M8", qApp->translate("DataProbe", "Mining Probe G8")},
    {"M9", qApp->translate("DataProbe", "Mining Probe G9")},
    {"M10", qApp->translate("DataProbe", "Mining Probe G10")},
    {"R1", qApp->translate("DataProbe", "Research Probe G1")},
    {"R2", qApp->translate("DataProbe", "Research Probe G2")},
    {"R3", qApp->translate("DataProbe", "Research Probe G3")},
    {"R4", qApp->translate("DataProbe", "Research Probe G4")},
    {"R5", qApp->translate("DataProbe", "Research Probe G5")},
    {"R6", qApp->translate("DataProbe", "Research Probe G6")},
    {"B1", qApp->translate("DataProbe", "Booster Probe G1")},
    {"B2", qApp->translate("DataProbe", "Booster Probe G2")},
    {"S", qApp->translate("DataProbe", "Storage Probe")},
    {"D", qApp->translate("DataProbe", "Duplicator Probe")},
    {"B", qApp->translate("DataProbe", "Basic Probe")},
    {"CF", qApp->translate("DataProbe", "Fuel Recovery Probe")},
    {"CM", qApp->translate("DataProbe", "Melee Attack Probe")},
    {"CR", qApp->translate("DataProbe", "Ranged Attack Probe")},
    {"CD", qApp->translate("DataProbe", "EZ Debuff Probe")},
    {"CA", qApp->translate("DataProbe", "Attribute Resistance Probe")},
};

QString dataProbeName(Probe::Ptr probe) { return kProbeNames.value(probe->id); }

void sortProbeInventory(ProbeInventory &probeInventory) {
  std::ranges::sort(probeInventory, [](const ProbeInventory::value_type &lhs,
                                       const ProbeInventory::value_type &rhs) {
    return Probe::fromString(lhs.first) < Probe::fromString(rhs.first);
  });
}
