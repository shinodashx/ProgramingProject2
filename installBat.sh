git clone https://github.com/andlabs/libui.git
cd libui
meson setup build
ninja -C build
install *.a build/lib/libui.a
