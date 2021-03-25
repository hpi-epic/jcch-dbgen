set(DATABASE VECTORWISE)
set(MACHINE LINUX)
set(WORKLOAD TPCH)

add_library(
    jcch-dbgen

    STATIC
    build.c
    config.h
    driver.c
    dss.h
    dsstypes.h
    jcch_dbgen.c
    bm_utils.c
    release.h
    rnd.c
    load_stub.c
    speed_seed.c
    skew/phash.h
    skew/phash.c
    text.c
    permute.c
    print.c
    rng64.c
    shared.h
)

target_include_directories(
    jcch-dbgen

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(
    jcch-dbgen

    PUBLIC
    -DDBNAME="dss"
    -D${MACHINE}
    -D${DATABASE}
    -D${WORKLOAD}
    -D_FILE_OFFSET_BITS=64
    -DJCCH_SKEW=1
)

target_compile_options(
    jcch-dbgen

    PRIVATE
    -Wno-unused-result
    -Wno-format
    -Wno-dangling-else
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  target_compile_options(
    jcch-dbgen

    PRIVATE
    -Wno-string-plus-int
  )
endif()
