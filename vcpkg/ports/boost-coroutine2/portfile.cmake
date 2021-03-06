# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/coroutine2
    REF boost-1.79.0
    SHA512 a93c43fea67d2963e90d9e84e7a6b2832d1f6a9810385092198300af35f1d27b4a33367eff256c7bd32beed0041d84ada62a12aeda45f5f7641fc801b94f0aeb
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
