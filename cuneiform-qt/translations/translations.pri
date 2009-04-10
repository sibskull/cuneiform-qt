######################################################################
# Localization

TRANSLATIONS += \
			cuneiform-qt_ru.ts cuneiform-qt_uk.ts

COMPILED_TRANSLATIONS += \
			cuneiform-qt_ru.qm cuneiform-qt_uk.qm

DISTFILES += $$COMPILED_TRANSLATIONS

for(t, COMPILED_TRANSLATIONS):languages.files += "translations/$${t}"
languages.path  = $$TRANSLATIONS_DIR

INSTALLS += languages
