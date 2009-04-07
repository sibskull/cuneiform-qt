######################################################################
# Localization

TRANSLATIONS += \
			cuneiform-qt_ru.ts

COMPILED_TRANSLATIONS += \
			cuneiform-qt_ru.qm

DISTFILES += $$COMPILED_TRANSLATIONS

for(t, COMPILED_TRANSLATIONS):languages.files += "translations/$${t}"
languages.path  = $$TRANSLATIONS_DIR

INSTALLS += languages
