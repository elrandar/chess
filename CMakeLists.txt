# OPTIMISATION FLAGS : FIXME
# In cmake, we define variables with set(VAR Value [Value]*)
# set(MYVAR "val1") assigns value val1 to MYVAR
# set(MYVAR "val1" "val2") creates a list

set(OPT_FLAGS
    "-O2"
)

# END OPTIMIZATION FLAGS
# SOURCES : FIXME
# Put all your sources here EXCEPT YOUR MAIN.CC
set(SRC_ENGINE
        "src/chess_engine/board/chessboard-representation.cc"
        "src/chess_engine/board/chessboard-representation.hh"
        "src/chess_engine/board/move.cc"
        "src/chess_engine/board/move.hh"
        "src/chess_engine/board/chessboard.hh"
        "src/chess_engine/board/chessboard.cc"
        "src/chess_engine/board/masks.cc"
        "src/chess_engine/board/bitboard-operations.cc"
        "src/chess_engine/board/bitboard-operations.hh"
        "src/chess_engine/board/magic.cc"
        "src/chess_engine/board/magic.hh"
        "src/chess_engine/board/rule.cc"
        "src/parsing/option.cc"
        "src/parsing/option.hh"
        "src/parsing/pgn-parser/pgn-parser.cc"
        "src/parsing/pgn-parser/pgn-exception.cc"
        "src/parsing/pgn-parser/report-type.hh"
        "src/parsing/pgn-parser/pgn-move.cc"
        "src/listener-manager.cc"
        "src/listener/listener-wrapper.cc"
        "src/parsing/perft-parser/fen-object.cc"
        "src/parsing/perft-parser/fen-rank.cc"
        "src/parsing/perft-parser/perft-object.cc"
        "src/perft.cc"
        "src/chess_engine/ai/uci.cc"
        "src/chess_engine/ai/evaluation.cc"
        "src/chess_engine/ai/search.cc"
        "src/chess_engine/ai/gtree.cc"
        "src/chess_engine/ai/ai.cc"
        "src/chess_engine/board/zobrist.cc"
        "src/chess_engine/ai/tools.cc"
        "src/chess_engine/ai/tools.hh"
        )

# Put your test files here
set(SRC_GTEST
        "tests/unit/chessboard-representation-tests.cc"
        "tests/unit/magic-tests.cc"
        "tests/unit/option-tests.cc"
        "tests/unit/chessboard-tests.cc"
        "tests/unit/perft-parsing-tests.cc"
        "tests/unit/evaluation-tests.cc"
        "tests/unit/move-generation-tests.cc"
        "tests/unit/search-tests.cc"
        "tests/unit/ai-tests.cc"
        "tests/unit/hashing-tests.cc"
)

# END SOURCES

# COVERAGE FLAGS

set(COV_FLAGS
        ""
)

# END COVERAGE FLAGS

# SETUP
cmake_minimum_required(VERSION 3.12)
project(Chess)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -g3 ${COV_FLAGS}")

# LIBRARIES
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})

# CHESSENGINE
add_executable(chessengine)
target_sources(chessengine PRIVATE "src/main.cc" ${SRC_ENGINE})
target_link_libraries(chessengine PRIVATE ${LIBRARIES})


# TESTS
# Check if GTest is installed
find_package(GTest)

# If it is, create a dedicated target "tests"
if(GTest_FOUND)
  include(GoogleTest)

  add_executable(tests)
  target_sources(tests PRIVATE
    ${SRC_GTEST}
    ${SRC_ENGINE}
  )
  target_compile_options(tests PRIVATE -Wall -Werror -Wextra)
  target_link_libraries(tests PRIVATE GTest::GTest gtest_main ${LIBRARIES})
endif()

# STATIC TARGET
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(chessengine-static)
    target_sources(chessengine-static PRIVATE "src/main.cc" ${SRC_ENGINE})
    target_link_libraries(chessengine-static PRIVATE ${LIBRARIES} -static)
endif()
