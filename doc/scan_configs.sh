#!/bin/bash

# grep -r '..' -e 'CONFIG_.*'
# ack --group -o "CONFIG_LEPTO[A-Z,_]*"^C

grep -r '..' -e 'CONFIG_[A-Z,_]*'

#--- Fin ----------------------------------------------------------------------
