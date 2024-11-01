file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB SOURCE_HEADERS CONFIGURE_DEPENDS *.hpp)

add_library(interview_lib STATIC ${SOURCE_FILES} ${SOURCE_HEADERS})
target_include_directories(interview_lib PRIVATE .venv/include)

#                                 ...ooooo.         ..---##o
#                           .--^""#########o ..o--"  .o#####
#         .."-.         ..-^"  .o###########^"    .o########
#       ..."^o ^.    .o^"    o##^"#"#"#"#"##   .o#^:^:^:^:#
#         "^-:^.# .o#:-    o^"    "-"-"-"-##.o^"-^" " " :# .-^""
#            ::#"##o#^.  -""..---------...":^-------. o#^.^.#:"
#          -:.-:^:"oo^   .-"             ""o         ^:^#:#"
#      ..o:^o:#o:##""  o"                  "o          # "-o.
#     o"." ^"-"^-""   #                     #           #  .#.
#   .^o"o:-. .        #                    .^           #o-#-#
#  .#^.^.^#^#^        #                ...o:      .....o""o^o"#
#  #^ "-"- "           #""""^####^""""".-"#####^""""  o"  ^.^.^o
#  "#"o"o       .#--""" ^..       ..oo###oo.      ..-"    o:o"o^
#   ^-o:o".   .^"          """""""  ::^^^^^::"""""        ^.#-#
#     "^o:-:-#   -        ..    .o-"         "--..    .    ^.^
#       "-:-:^o            """"""                 """"     o"
#          "^-^#.   .o^#...........................    ..-"
#               """""   ""                         """"