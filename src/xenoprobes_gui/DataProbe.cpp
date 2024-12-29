/**
 * @file DataProbe.cpp
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#include "DataProbe.h"
#include <QApplication>
#include <unordered_map>

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

QString dataProbeName(Probe::Ptr probe) {
  static const std::unordered_map<Probe::Ptr, QString> kProbeNames{
      {Probe::M1, qApp->translate("DataProbe", "Mining Probe G1")},
      {Probe::M2, qApp->translate("DataProbe", "Mining Probe G2")},
      {Probe::M3, qApp->translate("DataProbe", "Mining Probe G3")},
      {Probe::M4, qApp->translate("DataProbe", "Mining Probe G4")},
      {Probe::M5, qApp->translate("DataProbe", "Mining Probe G5")},
      {Probe::M6, qApp->translate("DataProbe", "Mining Probe G6")},
      {Probe::M7, qApp->translate("DataProbe", "Mining Probe G7")},
      {Probe::M8, qApp->translate("DataProbe", "Mining Probe G8")},
      {Probe::M9, qApp->translate("DataProbe", "Mining Probe G9")},
      {Probe::M10, qApp->translate("DataProbe", "Mining Probe G10")},
      {Probe::R1, qApp->translate("DataProbe", "Research Probe G1")},
      {Probe::R2, qApp->translate("DataProbe", "Research Probe G2")},
      {Probe::R3, qApp->translate("DataProbe", "Research Probe G3")},
      {Probe::R4, qApp->translate("DataProbe", "Research Probe G4")},
      {Probe::R5, qApp->translate("DataProbe", "Research Probe G5")},
      {Probe::R6, qApp->translate("DataProbe", "Research Probe G6")},
      {Probe::B1, qApp->translate("DataProbe", "Booster Probe G1")},
      {Probe::B2, qApp->translate("DataProbe", "Booster Probe G2")},
      {Probe::S, qApp->translate("DataProbe", "Storage Probe")},
      {Probe::D, qApp->translate("DataProbe", "Duplicator Probe")},
      {Probe::B, qApp->translate("DataProbe", "Basic Probe")},
      {Probe::CF, qApp->translate("DataProbe", "Fuel Recovery Probe")},
      {Probe::CM, qApp->translate("DataProbe", "Melee Attack Probe")},
      {Probe::CR, qApp->translate("DataProbe", "Ranged Attack Probe")},
      {Probe::CD, qApp->translate("DataProbe", "EZ Debuff Probe")},
      {Probe::CA, qApp->translate("DataProbe", "Attribute Resistance Probe")},
  };
  return kProbeNames.at(probe);
}
