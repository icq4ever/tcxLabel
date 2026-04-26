# =============================================================================
# tcxLabel - Label / text rendering on top of TrussC's Font class
# =============================================================================

cmake_minimum_required(VERSION 3.20)
message(STATUS "[tcxLabel] Loading addon...")

# INTERFACE library — header-only addon. Just expose include path.
add_library(tcxLabel INTERFACE)
target_include_directories(tcxLabel INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# -----------------------------------------------------------------------------
# Bundle font files: copy addon's data/fonts/ into the consuming project's
# bin/data/fonts/ at configure time. The user's project loads them via
# Font::load("fonts/<name>.ttf", size) which resolves through getDataPath().
# -----------------------------------------------------------------------------
set(_TCXLABEL_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/data/fonts")
set(_TCXLABEL_DEST "${CMAKE_SOURCE_DIR}/bin/data/fonts")

if(EXISTS "${_TCXLABEL_SRC}")
    file(MAKE_DIRECTORY "${_TCXLABEL_DEST}")
    file(GLOB _TCXLABEL_FILES
        "${_TCXLABEL_SRC}/*.ttf" "${_TCXLABEL_SRC}/*.ttc" "${_TCXLABEL_SRC}/*.otf"
        "${_TCXLABEL_SRC}/OFL.txt" "${_TCXLABEL_SRC}/NOTICE.md")
    foreach(_f ${_TCXLABEL_FILES})
        get_filename_component(_fname "${_f}" NAME)
        if(NOT EXISTS "${_TCXLABEL_DEST}/${_fname}")
            file(COPY "${_f}" DESTINATION "${_TCXLABEL_DEST}")
            message(STATUS "[tcxLabel] Bundled font: ${_fname}")
        endif()
    endforeach()
endif()

message(STATUS "[tcxLabel] Addon loaded")
