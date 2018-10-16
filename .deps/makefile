GCC      = $(PREFIX)/bin/$(TARGET)-gcc
BINUTILS = $(PREFIX)/bin/$(TARGET)-as
MAKE	 = make --quiet

.PHONY: all clean distclean
all: $(GCC) $(BINUTILS)

$(GCC): gcc-$(GCC_VERSION) $(BINUTILS)
	mkdir -p build-gcc-$(GCC_VERSION) \
		&& cd build-gcc-$(GCC_VERSION) \
	&& export PATH=$(PREFIX)/bin:${PATH} \
		&& ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --prefix="$(PREFIX)" \
			--disable-nls --enable-languages=c --without-headers \
	&& $(MAKE) all-gcc \
	&& $(MAKE) all-target-libgcc \
	&& $(MAKE) install-gcc \
	&& $(MAKE) install-target-libgcc

$(BINUTILS): binutils-$(BINUTILS_VERSION)
	cd binutils-$(BINUTILS_VERSION) \
		&& ./configure --target=$(TARGET) --prefix="$(PREFIX)" \
			--with-sysroot --disable-nls --disable-werror \
		&& $(MAKE) \
		&& $(MAKE) install

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.gz
	tar -xzf gcc-$(GCC_VERSION).tar.gz

binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION).tar.gz
	tar -xzf binutils-$(BINUTILS_VERSION).tar.gz

gcc-$(GCC_VERSION).tar.gz:
	wget \
		https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.gz

binutils-$(BINUTILS_VERSION).tar.gz:
	wget https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.gz


clean:
	$(MAKE) --directory gcc-$(GCC_VERSION) clean
	$(MAKE) --directory binutils-$(BINUTILS_VERSION) clean
	rm -rf $(PREFIX)

distclean:
	rm -rf $(PREFIX) gcc-$(GCC_VERSION) binutils-$(BINUTILS_VERSION) \
		gcc-$(GCC_VERSION).tar.gz binutils-$(BINUTILS_VERSION).tar.gz
