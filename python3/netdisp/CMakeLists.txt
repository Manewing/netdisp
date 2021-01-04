pyfile("__init__")
pyfile("command")
pyfile("netdisp")
pyfile("udpsender")

add_custom_target(
  python3_netdisp
  ALL
  DEPENDS
  ${PYFILES}
)

add_custom_target(
  python3_netdisp_install
  COMMAND
    echo ${PYFILES}
  DEPENDS
  ${PYFILES}
)