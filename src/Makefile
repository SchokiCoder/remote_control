#    remote_control
#    Copyright (C) 2021  Andy Frank Schoknecht
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC = cc
APP_NAME = remote_control
CFLAGS = -std=c99 -Wall -pedantic -O3
LIBS = -l SDL2 -l SDL2_image

INSTALL_BIN_DIR = /usr/bin
INSTALL_ASSETS_DIR = /usr/share/${APP_NAME}
INSTALL_TEXTURES_DIR = ${INSTALL_ASSETS_DIR}/textures

DEFINES = -D PATH_ASSETS="\"${INSTALL_ASSETS_DIR}/\"" -D PATH_TEXTURES="\"${INSTALL_TEXTURES_DIR}/\""

options:
	@echo ${APP_NAME} build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "CC       = ${CC}"
	@echo "LIBS     = ${LIBS}"

clean:
	rm -f ${APP_NAME} *.o

install:
	#compile
	${CC} *.c ${CFLAGS} ${LIBS} -o ${APP_NAME} ${DEFINES}

	#move bin to install dir
	mkdir -p ${INSTALL_BIN_DIR}
	mv -f ${APP_NAME} ${INSTALL_BIN_DIR}
	chmod 755 ${INSTALL_BIN_DIR}/${APP_NAME}

	#move assets
	mkdir -p ${INSTALL_ASSETS_DIR}
	mkdir -p ${INSTALL_TEXTURES_DIR}
	cp ../assets/textures/* ${INSTALL_TEXTURES_DIR}

uninstall:
	#remove bin
	rm -f ${INSTALL_BIN_DIR}/${APP_NAME}

	#remove assets
	rm -f -r ${INSTALL_ASSETS_DIR}