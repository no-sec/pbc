cmake_minimum_required(VERSION 3.0)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_MODULE_PATH ${CMAKE_PROJECT_DIR})
PROJECT(pbc)
enable_testing()

include(FindGMP.cmake)

find_package(BISON 2.3)
find_package(FLEX 2.5)
#find_package(GMP 5.1.0)

set(warnflags "-Wall -W -Wfloat-equal -Wendif-labels -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wredundant-decls")
set(optflags  "-O3 -pipe -ffast-math -fomit-frame-pointer")
set(CMAKE_C_FLAGS "${warnflags} ${optflags}")

FLEX_TARGET(pbcscanner pbc/parser.lex ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c COMPILE_FLAGS "--header-file=pbc/lex.yy.h")
BISON_TARGET(pbcparser pbc/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c COMPILE_FLAGS "-b pbc/parser")
ADD_FLEX_BISON_DEPENDENCY(pbcscanner pbcparser)

include_directories("include" "pbc" "." "${GMP_INCLUDES}" )

set(libpbc_srcs
  arith/field.c
  arith/fp.c
  arith/montfp.c
  arith/naivefp.c
  arith/fastfp.c
  arith/fasterfp.c
  arith/multiz.c
  arith/z.c
  arith/fieldquadratic.c
  arith/poly.c
  arith/ternary_extension_field.c
  arith/random.c
  arith/dlog.c

  ecc/curve.c
  ecc/singular.c
  ecc/pairing.c
  ecc/param.c
  ecc/a_param.c
  ecc/d_param.c
  ecc/e_param.c
  ecc/f_param.c
  ecc/g_param.c
  ecc/eta_T_3.c
  ecc/hilbert.c
  ecc/mnt.c
  ecc/mpc.c

  misc/utils.c
  misc/darray.c
  misc/symtab.c
  misc/extend_printf.c
  misc/memory.c

  misc/get_time.c
  arith/init_random.c )

set(bin_srcs
  example/bls.c
  example/hess.c
  example/joux.c
  example/paterson.c
  example/yuanli.c
  example/zhangkim.c
  example/zss.c

  gen/gena1param.c
  gen/genaparam.c
  gen/gendparam.c
  gen/geneparam.c
  gen/genfparam.c
  gen/gengparam.c
  gen/hilbertpoly.c
  gen/listmnt.c
  gen/listfreeman.c

  benchmark/benchmark.c
  benchmark/timersa.c
  benchmark/ellnet.c
  benchmark/multipairing.c )

add_library(libpbc STATIC ${libpbc_srcs})
target_link_libraries(libpbc ${GMP_LIBRARIES})
set_target_properties(libpbc  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION C
  MACOSX_FRAMEWORK_IDENTIFIER crypto.stanford.edu.pbc
  # MACOSX_FRAMEWORK_INFO_PLIST Info.plist
  PUBLIC_HEADER pbc.h
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)

add_executable(pbc
  pbc/pbc.c
  ${FLEX_pbcscanner_OUTPUTS}
  ${BISON_pbcparser_OUTPUTS}
  pbc/pbc_getline.readline.c)
target_link_libraries(pbc libpbc readline)

set(fp_srcs
  arith/field.c
  arith/fp.c
  arith/naivefp.c
  arith/fastfp.c
  arith/fasterfp.c
  arith/montfp.c
  arith/random.c
  arith/init_random.c
  misc/extend_printf.c
  misc/memory.c
  misc/utils.c
  arith/multiz.c
  misc/darray.c )

set(poly_srcs
  arith/poly.c
  misc/darray.c)

set(quadratic_srcs
  arith/fieldquadratic.c )

foreach(test fp quadratic poly exp prodpairing)
  add_executable(${test}_test guru/${test}_test.c ${${test}_srcs})
  target_link_libraries(${test}_test libpbc)
  add_test(NAME ${test} COMMAND "${test}_test")
endforeach(test)

