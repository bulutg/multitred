#!/bin/bash

ss -apt state established | grep Partner | awk '{print $3,$5}'