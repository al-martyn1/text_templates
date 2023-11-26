#ifndef SETTINGS_H
#define SETTINGS_H

#include "cfarcls.h"

struct InitDialogItem
{
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned char Focus;
  unsigned int Selected;
  unsigned int Flags;
  unsigned char DefaultButton;
  char *Data;
};

void InitDialogItems(const struct InitDialogItem *Init,struct FarDialogItem *Item,
                     int ItemsNumber);

namespace TextTP
{

int getConfigInputMethod();
BOOL getConfigAutoComplete();
BOOL getConfigShowErrors();
BOOL getConfigAllowEditorSettingsChangeFeature();
BOOL getConfigDisablePreloading();
BOOL getConfigAllowReloadEditorFiles();
BOOL getConfigReloadEditorFilesFeatureDontAskUser();

void settingsDialog();


};




#endif /* SETTINGS_H */