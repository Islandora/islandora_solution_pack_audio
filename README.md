# Audio Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_audio.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_audio)

## Introduction

Adds all required Fedora objects to allow users to ingest and retrieve audio files through the Islandora interface

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Lame](http://lame.sourceforge.net) (Debian/Ubuntu `sudo apt-get install lame`)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set the path for `lame` and select a viewer in Administration » Islandora » Audio Collection (admin/islandora/audio).

An option also exists to fall back to the OBJ datastream in viewers if the PROXY_MP3 datastream doesn't exist and the OBJ can be played.

![Configuration](https://cloud.githubusercontent.com/assets/3406327/11278393/1f340714-8ec1-11e5-8d08-3bce15529b42.png)

Current viewers that can be configured include:

* [Islandora JW Player](https://github.com/Islandora/islandora_jwplayer)
* [Islandora Video.js](https://github.com/Islandora/islandora_videojs)

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Audio+Solution+Pack).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors
Current maintainers:

* [Rosemary Le Faive](https://github.com/rosiel)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
