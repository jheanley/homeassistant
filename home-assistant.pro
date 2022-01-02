TEMPLATE = subdirs

libraries.file   = libraries/libraries.pro
libraries.target = libraries

KitchenLight.file = KitchenLight/KitchenLight.pro
KitchenLight.target = KitchenLight

kitchen-blind.file = kitchen-blind/kitchen-blind.pro
kitchen-blind.target = kitchen-blind

SUBDIRS = gaming-desk \
          libraries \
          KitchenLight \
          kitchen-blind

win32 {
    HOMEDIR += $$(USERPROFILE)
}
else
{
    HOMEDIR += $$(HOME)
}

INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoavr/cores/arduino"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-atmelavr/avr/include"
