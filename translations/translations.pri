TRANSLATIONS += \
    translations/harbour-kasa.ts \
    translations/harbour-kasa-de.ts \
    translations/harbour-kasa-es.ts \
    translations/harbour-kasa-fr.ts \
    translations/harbour-kasa-hu.ts \
    translations/harbour-kasa-pl.ts \
    translations/harbour-kasa-ru.ts \
    translations/harbour-kasa-sv.ts

qm.input    = TRANSLATIONS
qm.output   = translations/${QMAKE_FILE_BASE}.qm
qm.commands = @echo "compiling ${QMAKE_FILE_NAME}"; \
                lrelease -idbased -silent ${QMAKE_FILE_NAME} -qm ${QMAKE_FILE_OUT}
qm.CONFIG   = target_predeps no_link

QMAKE_EXTRA_COMPILERS += qm

translations.files = $$OUT_PWD/translations
translations.path  = $$PREFIX/share/$$TARGET

INSTALLS += translations
