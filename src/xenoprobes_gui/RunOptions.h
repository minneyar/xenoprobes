/**
 * @file RunOptions.h
 *
 * @author Dan Keenan
 * @date 11/30/24
 * @copyright GNU GPLv3
 */

#ifndef RUNOPTIONS_H
#define RUNOPTIONS_H
#include <QThread>

struct RunOptions {
  int storageWeight = 1000;
  int revenueWeight = 10;
  int productionWeight = 1;
  int iterations = 2000;
  int population = 200;
  int offsprings = 100;
  int mutation = 3;
  int threads = QThread::idealThreadCount();
};

#endif //RUNOPTIONS_H
