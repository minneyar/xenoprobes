/**
 * @file settings.h
 *
 * @author Dan Keenan
 * @date 12/7/24
 * @copyright GNU GPLv3
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStandardPaths>

#define XP_SETTING(T, name, defaultValue)                                      \
  [[nodiscard]] T get##name() {                                                \
    return settings::detail::getSetting<T>(#name, defaultValue);               \
  }                                                                            \
  void set##name(T value) {                                                    \
    settings::detail::setSetting(#name, std::forward<T>(value));               \
  }                                                                            \
  T default##name() { return defaultValue; }

namespace settings {

namespace detail {
template <typename T> T getSetting(const char *name, T defaultValue) {
  QSettings settings;
  return settings.value(name, std::forward<T>(defaultValue))
      .template value<T>();
}
template <typename T> void setSetting(const char *name, T value) {
  QSettings settings;
  settings.setValue(name, std::forward<T>(value));
}

} // namespace detail

XP_SETTING(QByteArray, MainWindowGeometry, {});
XP_SETTING(QString, LastFileDialogPath,
           QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
XP_SETTING(QStringList, RecentDocuments, {});

} // namespace settings

#undef XP_SETTING

#endif // SETTINGS_H
