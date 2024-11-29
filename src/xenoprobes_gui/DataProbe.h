/**
 * @file DataProbe.h
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#ifndef DATAPROBE_H
#define DATAPROBE_H

#include <QHash>
#include <QString>

struct DataProbe {
  friend constexpr std::weak_ordering operator<=>(const DataProbe &lhs,
                                                  const DataProbe &rhs) {
    if (lhs.category != rhs.category) {
      return lhs.category <=> rhs.category;
    }
    return lhs.level <=> rhs.level;
  }

  using ProbeInventory = QList<QPair<QString, unsigned int>>;

  enum class Category {
    Basic,
    Mining,
    Research,
    Booster,
    Storage,
    Duplicator,
    Battle,
  };

  QString id;
  Category category;
  unsigned int level;
  QString name;
  [[nodiscard]] QString icon() const {
    switch (category) {
    case Category::Basic:
      return ":/probe_icons/basic.png";
    case Category::Mining:
      return ":/probe_icons/mining.png";
    case Category::Research:
      return ":/probe_icons/research.png";
    case Category::Booster:
      return ":/probe_icons/booster.png";
    case Category::Storage:
      return ":/probe_icons/storage.png";
    case Category::Duplicator:
      return ":/probe_icons/duplicator.png";
    case Category::Battle:
      return ":/probe_icons/battle.png";
    }
    Q_UNREACHABLE();
  }

  static const QHash<QString, DataProbe> kAllProbes;
};

#endif // DATAPROBE_H
