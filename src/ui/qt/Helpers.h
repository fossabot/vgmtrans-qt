#ifndef VGMTRANS_HELPERS_H
#define VGMTRANS_HELPERS_H

#include <QIcon>
#include "VGMFile.h"

QIcon iconForFileType(FileType filetype);
QColor colorForEventColor(uint8_t eventColor);
QColor textColorForEventColor(uint8_t eventColor);
wchar_t *qstringTowchar_t(QString text);
// std::vector<wchar_t> qstringTowchar_t(QString text);

#endif  // VGMTRANS_HELPERS_H
