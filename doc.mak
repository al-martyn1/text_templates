# !include "incdirs.mak"

# UDFLAGS = -I$(UD_INC)
# UDFLAGS = -d9 -K+
UDFLAGS =
# UDFLAGS = -K+

.suffixes: .udf .rtf

XML2UDF = perl.exe F:\usr\bin\xml2udf.pl
CC2X = cc2x.exe

DOCSRCROOT=.\doc.src
DOCROOT=.\doc

#.path.udf = $(DOCSRCROOT)
#.path.rtf = $(DOCROOT)

#.udf.rtf:
#    @echo Compiling $<
#    ud.exe $(UDFLAGS) -DRTF -o$@ $<


HOTKEYS_DEPENDENCES = $(DOCSRCROOT)\hotkeys-ru.udf
OLDHOTKEYS_DEPENDENCES = $(DOCSRCROOT)\old-hotkeys-ru.udf
UG_DEPENDENCES = $(DOCSRCROOT)\ttp.udf                           \
                 $(DOCSRCROOT)\cpp-styles.cpp                    \
                 $(DOCSRCROOT)\text-fmt.txt                      \
                 $(DOCSRCROOT)\comments.xml                      \
                 $(DOCSRCROOT)\file-types.xml                    \
                 $(DOCSRCROOT)\shortcuts.xml                     \
                 $(DOCSRCROOT)\templates.xml                     \
                 $(DOCSRCROOT)\texttp-custom-info-sample.xml     \
                 $(DOCSRCROOT)\texttp-sample.xml

#                 $(DOCSRCROOT)\




hotkeys  : $(HOTKEYS_DEPENDENCES)  doc\hotkeys-ru.rtf
old-hotkeys  : $(OLDHOTKEYS_DEPENDENCES)  doc\old-hotkeys-ru.rtf
ttp_help : doc\ttp.hlp
ttp_rtf : doc\ttpug.rtf




doc\ttp.hlp :  $(UG_DEPENDENCES)
             ud.exe $(UDFLAGS) -DWINHELP -odoc\ttp.rtf doc.src\ttp.udf
             hcw.exe /C /M /E doc\ttp.hpj
#             -@copy  /Y doc\udp.h h\udp_wh.h

doc\ttpug.rtf    :  $(UG_DEPENDENCES)
             ud.exe $(UDFLAGS) -DRTF -odoc\ttpug.rtf doc.src\ttp.udf

doc\hotkeys-ru.rtf: $(HOTKEYS_DEPENDENCES)
             ud.exe $(UDFLAGS) -DRTF -odoc\hotkeys-ru.rtf doc.src\hotkeys-ru.udf

doc\old-hotkeys-ru.rtf: $(OLDHOTKEYS_DEPENDENCES)
             ud.exe $(UDFLAGS) -DRTF -odoc\old-hotkeys-ru.rtf doc.src\old-hotkeys-ru.udf


# Preparing XML files to be included
# $(DOCSRCROOT)\os.xml.udf : $(DOCSRCROOT)\os.xml
#                $(XML2UDF) < $(DOCSRCROOT)\os.xml > $(DOCSRCROOT)\os.xml.udf


#$(DOCSRCROOT)\texttp-custom-info-sample.xml.udf: $(DOCSRCROOT)\texttp-custom-info-sample.xml
#             $(XML2UDF) < $(DOCSRCROOT)\texttp-custom-info-sample.xml > $(DOCSRCROOT)\texttp-custom-info-sample.xml.udf
#
#$(DOCSRCROOT)\texttp-sample.xml.udf: $(DOCSRCROOT)\texttp-sample.xml
#             $(XML2UDF) < $(DOCSRCROOT)\texttp-sample.xml > $(DOCSRCROOT)\texttp-sample.xml.udf
#
#$(DOCSRCROOT)\templates.xml.udf: $(DOCSRCROOT)\templates.xml
#             $(XML2UDF) < $(DOCSRCROOT)\templates.xml > $(DOCSRCROOT)\templates.xml.udf
