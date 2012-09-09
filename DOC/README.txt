Copyright 2012 Sheldon Bachstein

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.


The avl Library is a set of object-oriented subsystems which may be
combined into a cohesive simulation engine. The design goal focuses
on:
-Ease of use
-Cleanliness of the API interfaces
-Extensibility

The library is composed of five subsystems:
-utility:	Provides the underlying infrastructure of the
		library in addition to commonly-needed
		functionality.
-video:		Provides windowing and two-dimensionsonal graphical
		capabilities.
-sound:		Provides audio capabilities.
-input:		Provides capabilities for receiving user input using
		an event-based paradigm.
-model:		Provides an infrastructure for representing abstract
		entities in a uniform, manageable way.

Each subsystem may be used indepedent of the others, except that
each subsystem depends upon the utility subsystem.