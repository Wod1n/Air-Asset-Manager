#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include "ui_transferDialog.h"

class TransferDialog : public QDialog{

public:
    explicit TransferDialog(QDialog *parent = nullptr);
    int getNewRegion();

private:
    Ui::TransferDialog ui;
};

#endif
