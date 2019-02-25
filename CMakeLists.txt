cmake_minimum_required(VERSION 3.0)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_MODULE_PATH ${CMAKE_PROJECT_DIR})
PROJECT(pbc)
enable_testing()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
find_package(GMP REQUIRED)

find_package(BISON 2.3)
find_package(FLEX 2.5)

add_compile_options("-Wall")
add_compile_options("-W")
add_compile_options("-Wfloat-equal")
add_compile_options("-Wendif-labels")
add_compile_options("-Wshadow")
add_compile_options("-Wpointer-arith")
add_compile_options("-Wcast-align")
add_compile_options("-Wstrict-prototypes")
add_compile_options("-Wredundant-decls")
#add_compile_options("-O3")
add_compile_options("-O0")
add_compile_options("-g")
add_compile_options("-pipe")
add_compile_options("-ffast-math")
add_compile_options("-fembed-bitcode")

FLEX_TARGET(pbcscanner pbc/parser.lex ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c COMPILE_FLAGS "--header-file=pbc/lex.yy.h")
BISON_TARGET(pbcparser pbc/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c COMPILE_FLAGS "-b pbc/parser")
ADD_FLEX_BISON_DEPENDENCY(pbcscanner pbcparser)

foreach(ARCH i386 x86_64 arm64 armv7)
set(EXTRAS "-arch ${ARCH}")

if(${ARCH} STREQUAL "i386")
    set(SHORTSDK "iphonesimulator")
elseif(${ARCH} STREQUAL "armv7")
    set(SHORTSDK "iphoneos")
elseif(${ARCH} STREQUAL "arm64")
    set(SHORTSDK "iphoneos")
elseif(${ARCH} STREQUAL "x86_64")
    set(SHORTSDK "macosx")
endif(${ARCH} STREQUAL "i386")

execute_process(COMMAND "xcrun" "--sdk" ${SHORTSDK} "--find" "clang"           OUTPUT_VARIABLE CMAKE_C_COMPILER   OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND "xcrun" "--sdk" ${SHORTSDK} "--find" "clang++"         OUTPUT_VARIABLE CMAKE_CXX_COMPILER OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND "xcrun" "--sdk" ${SHORTSDK} "--show-sdk-platform-path" OUTPUT_VARIABLE PLATFORM           OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND "xcrun" "--sdk" ${SHORTSDK} "--show-sdk-path"          OUTPUT_VARIABLE SDK                OUTPUT_STRIP_TRAILING_WHITESPACE)

set(MIN_IOS "9.0")

if(NOT ${ARCH} STREQUAL "armv7")
  set(EXTRAS "${EXTRAS} -fomit-frame-pointer")
endif(NOT ${ARCH} STREQUAL "armv7")

if(NOT ${ARCH} STREQUAL "x86_64" AND NOT ${ARCH} STREQUAL "i386")
    set(EXTRAS "${EXTRAS} -miphoneos-version-min=${MIN_IOS} -no-integrated-as -target ${ARCH}-apple-darwin")
endif(NOT ${ARCH} STREQUAL "x86_64" AND NOT ${ARCH} STREQUAL "i386")

include_directories("include" "pbc" "." "gmp/gmplib-${ARCH}/include" )

set(libpbc_srcs
  arith/field.c arith/fp.c arith/montfp.c arith/naivefp.c arith/fastfp.c arith/fasterfp.c arith/multiz.c
  arith/z.c arith/fieldquadratic.c arith/poly.c arith/ternary_extension_field.c arith/random.c arith/dlog.c
  ecc/curve.c ecc/singular.c ecc/pairing.c ecc/param.c ecc/a_param.c ecc/d_param.c ecc/e_param.c
  ecc/f_param.c ecc/g_param.c ecc/eta_T_3.c ecc/hilbert.c ecc/mnt.c ecc/mpc.c
  misc/utils.c misc/darray.c misc/symtab.c misc/extend_printf.c misc/memory.c misc/get_time.c arith/init_random.c ) 

add_library(pbc-${ARCH} STATIC ${libpbc_srcs})
target_link_libraries(pbc-${ARCH} ${GMP_LIBRARY})
set_target_properties(pbc-${ARCH} PROPERTIES COMPILE_FLAGS "${EXTRAS} -isysroot ${SDK}")
endforeach(ARCH)

#set(bin_srcs
#  example/bls.c
#  example/hess.c
#  example/joux.c
#  example/paterson.c
#  example/yuanli.c
#  example/zhangkim.c
#  example/zss.c
#
#  gen/gena1param.c
#  gen/genaparam.c
#  gen/gendparam.c
#  gen/geneparam.c
#  gen/genfparam.c
#  gen/gengparam.c
#  gen/hilbertpoly.c
#  gen/listmnt.c
#  gen/listfreeman.c
#
#  benchmark/benchmark.c
#  benchmark/timersa.c
#  benchmark/ellnet.c
#  benchmark/multipairing.c )
#set_target_properties(libpbc  PROPERTIES
#  FRAMEWORK TRUE
#  FRAMEWORK_VERSION C
#  MACOSX_FRAMEWORK_IDENTIFIER crypto.stanford.edu.pbc
#  # MACOSX_FRAMEWORK_INFO_PLIST Info.plist
#  PUBLIC_HEADER pbc.h
#  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
#)
#set(APP_TYPE MACOSX_BUNDLE)

#add_executable(pbc
#  pbc/pbc.c
#  ${FLEX_pbcscanner_OUTPUTS}
#  ${BISON_pbcparser_OUTPUTS}
#  pbc/pbc_getline.readline.c)
#target_link_libraries(pbc libpbc readline)

#set(fp_srcs
#  arith/field.c
#  arith/fp.c
#  arith/naivefp.c
#  arith/fastfp.c
#  arith/fasterfp.c
#  arith/montfp.c
#  arith/random.c
#  arith/init_random.c
#  misc/extend_printf.c
#  misc/memory.c
#  misc/utils.c
#  arith/multiz.c
#  misc/darray.c )

#set(poly_srcs
#  arith/poly.c
#  misc/darray.c)

#set(quadratic_srcs
#  arith/fieldquadratic.c )

#foreach(test fp quadratic poly exp prodpairing)
#  add_executable(${test}_test guru/${test}_test.c ${${test}_srcs})
#  target_link_libraries(${test}_test libpbc)
#  add_test(NAME ${test} COMMAND "${test}_test")
#endforeach(test)

