default:
	rm -rf target
	cargo rustc -- -C link-arg=--script=./linker.ld -C code-model=small -C relocation-model=static
	arm-none-eabi-objcopy -O binary target/aarch64-unknown-none/debug/rust-os-doccumented ./kernel8.img
