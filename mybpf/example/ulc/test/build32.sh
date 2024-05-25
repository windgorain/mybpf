
for file in *.c
do
    echo "compile ${file}"
    clang -O2 -I ../../../h -target bpf -c ${file} -D IN_ULC_USER --target=armv7a-linux-gnueabihf -emit-llvm
    file_no_ext=$(basename "${file}" .c)
    llc -march=bpf -filetype=obj ${file_no_ext}.bc
done

