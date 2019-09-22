#!/usr/bin/python3
#	xmegaring - RS232-controlled LED ring
#	Copyright (C) 2019-2019 Johannes Bauer
#
#	This file is part of xmegaring.
#
#	xmegaring is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; this program is ONLY licensed under
#	version 3 of the License, later versions are explicitly excluded.
#
#	xmegaring is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with xmegaring; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	Johannes Bauer <JohannesBauer@gmx.de>

class CRCCCITT():
	_INIT_VALUE = 0xffff

	@classmethod
	def calculate(cls, data):
		crc = cls._INIT_VALUE
		for bytevalue in data:
			crc = cls._update(crc, bytevalue)
		return crc

	@classmethod
	def _update(cls, crc, data):
		data ^= crc & 0xff
		data ^= (data << 4) & 0xff
		return (((data << 8) & 0xffff) | ((crc >> 8) & 0xff)) ^ ((data >> 4) & 0xff) ^ ((data << 3) & 0xffff)

if __name__ == "__main__":
	assert(CRCCCITT.calculate(bytes.fromhex("")) == 0xffff)
	assert(CRCCCITT.calculate(bytes.fromhex("5a")) == 0xf258)
