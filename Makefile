#
# Copyright (C) 2006-2009 OpenWrt.org
# Copyright (C) 2016 okada
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=hello_driver
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/hello_driver
  SUBMENU:=Other modules
  # DEPENDS:=@TARGET_brcm47xx
  TITLE:=Hello Driver
  AUTOLOAD:=$(call AutoLoad,70,hello_driver)
  FILES:=$(PKG_BUILD_DIR)/hello_driver.ko
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	ARCH="$(LINUX_KARCH)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(BUILDFLAGS)" \
	modules
endef

$(eval $(call KernelPackage,hello_driver))
