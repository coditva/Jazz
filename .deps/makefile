GCC      = $(PREFIX)/bin/$(ARCH)-gcc
BINUTILS = $(PREFIX)/bin/$(ARCH)-as
MAKE	 = make --quiet

.PHONY: all clean distclean
all: $(BINUTILS) $(GCC)

$(GCC): src/$(ARCH)/gcc-$(GCC_VERSION) .GCC_PREREQUISITES $(BINUTILS)
	$(info Building $(@F))
	mkdir -p build/gcc-$(GCC_VERSION)-$(ARCH) \
		&& cd build/gcc-$(GCC_VERSION)-$(ARCH) \
	&& export PATH=$(PREFIX)/bin:${PATH} \
		&& ../../src/$(ARCH)/gcc-$(GCC_VERSION)/configure \
			--target=$(ARCH) \
			--prefix="$(PREFIX)" \
			--disable-nls \
			--enable-languages=c  \
			--with-newlib \
			--without-headers \
			--disable-nls \
			--disable-shared \
			--disable-multilib \
			--disable-decimal-float \
			--disable-threads \
			--disable-libatomic \
			--disable-libgomp \
			--disable-libquadmath \
			--disable-libssp \
			--disable-libvtv \
	&& $(MAKE) all-gcc \
	&& $(MAKE) install-gcc

.GCC_PREREQUISITES: src/$(ARCH)/gcc-$(GCC_VERSION)
	$(info Downloading prerequisites for $(@F))
	cd $< && ./contrib/download_prerequisites
	touch $@

$(BINUTILS): src/$(ARCH)/binutils-$(BINUTILS_VERSION)
	$(info Building $(@F))
	cd $< \
		&& ./configure --target=$(ARCH) --prefix="$(PREFIX)" \
			--with-sysroot --disable-nls --disable-werror \
		&& $(MAKE) \
		&& $(MAKE) install

src/$(ARCH)/gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.gz
	$(info Extracting $@)
	mkdir -p $(@D)
	tar -xzf $< --directory $(@D)
	touch $@

src/$(ARCH)/binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION).tar.gz
	$(info Extracting $@)
	mkdir -p $(@D)
	tar -xzf $< --directory $(@D)
	touch $@

gcc-$(GCC_VERSION).tar.gz:
	$(info Downloading $@)
	curl --silent https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/$@ --output $@

binutils-$(BINUTILS_VERSION).tar.gz:
	$(info Downloading $@)
	curl --silent https://ftp.gnu.org/gnu/binutils/$@ --output $@


clean:
	$(MAKE) --directory gcc-$(GCC_VERSION) clean
	$(MAKE) --directory binutils-$(BINUTILS_VERSION) clean
	rm -rf $(PREFIX)

distclean:
	rm -rf $(PREFIX) src/ .GCC_PREREQUISITESJ
