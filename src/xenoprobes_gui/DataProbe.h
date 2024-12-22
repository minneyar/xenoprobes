/**
 * @file DataProbe.h
 *
 * @author Dan Keenan
 * @date 11/24/24
 * @copyright GNU GPLv3
 */

#ifndef DATAPROBE_H
#define DATAPROBE_H

#include <vector>
#include <utility>
#include <QString>
#include <probeoptimizer/probe.h>

QString dataProbeIcon(Probe::Ptr probe);
QString dataProbeName(Probe::Ptr probe);

#endif // DATAPROBE_H
