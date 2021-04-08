 #include "transferDialog.h"

TransferDialog::TransferDialog(QDialog *parent) : QDialog(parent){
    ui.setupUi(this);
}

int TransferDialog::getNewRegion(){
  if(ui.fdButton->isChecked()){
    return 0;
  }

  else if(ui.hgButton->isChecked()){
    return 1;
  }

  else if(ui.omButton->isChecked()){
    return 2;
  }

  else if(ui.itButton->isChecked()){
    return 3;
  }

  else{
    return -1;
  }
}
