#include "utils.h"

int Utils::getWrappedLines(String (&lines)[MAX_WRAPPED_LINES], String str, int limit) {
    char buf[str.length() + 1];
    char lineBuf[limit + 1];
    str.toCharArray(buf, str.length() + 1);

    char *p = buf;
    char *eol;
    int lineCount = 0;
    for (int i = 0; i < MAX_WRAPPED_LINES; i++) {
        if (p - buf > strlen(buf)) {
            lines[i] = "";
            continue;
        }
        eol = strchr(p, '\n');
        if (eol == NULL) {
            eol = p + strlen(p);
        }

        if (eol - p > limit) {
            eol = p + limit;
            while (*eol != ' ' && *eol != '\n' && eol > p) {
                eol--;
            }
        }
        strncpy(lineBuf, p, eol - p);
        lineBuf[eol - p] = '\0';

        lines[i] = String(lineBuf);
        lineCount++;
        p = eol + 1;
    }
    return lineCount;
}

String Utils::getWrappedLine(String str, int limit, int lineNum, int maxLines) {
    if (lineNum > maxLines) {
        return "";
    }
    char buf[str.length() + 1];
    char lineBuf[limit + 1];
    str.toCharArray(buf, str.length() + 1);

    String lines[maxLines];

    char *p = buf;
    char *eol;

    for (int i = 0; i < maxLines && i <= lineNum; i++) {
        if (p - buf > strlen(buf)) {
            lines[i] = "";
            continue;
        }
        eol = strchr(p, '\n');
        if (eol == NULL) {
            eol = p + strlen(p);
        }

        if (eol - p > limit) {
            eol = p + limit;
            while (*eol != ' ' && *eol != '\n' && eol > p) {
                eol--;
            }
        }
        strncpy(lineBuf, p, eol - p);
        lineBuf[eol - p] = '\0';

        lines[i] = String(lineBuf);
        p = eol + 1;
    }
    return lines[lineNum];
}

int32_t Utils::stringToColor(String color) {
    color.toLowerCase();
    color.replace(" ", "");
    if (color == "black") {
        return TFT_BLACK;
    } else if (color == "navy") {
        return TFT_NAVY;
    } else if (color == "darkgreen") {
        return TFT_DARKGREEN;
    } else if (color == "darkcyan") {
        return TFT_DARKCYAN;
    } else if (color == "maroon") {
        return TFT_MAROON;
    } else if (color == "purple") {
        return TFT_PURPLE;
    } else if (color == "olive") {
        return TFT_OLIVE;
    } else if (color == "lightgrey" || color == "grey") {
        return TFT_LIGHTGREY;
    } else if (color == "darkgrey") {
        return TFT_DARKGREY;
    } else if (color == "blue") {
        return TFT_BLUE;
    } else if (color == "green") {
        return TFT_GREEN;
    } else if (color == "cyan") {
        return TFT_CYAN;
    } else if (color == "red") {
        return TFT_RED;
    } else if (color == "magenta") {
        return TFT_MAGENTA;
    } else if (color == "yellow") {
        return TFT_YELLOW;
    } else if (color == "white") {
        return TFT_WHITE;
    } else if (color == "orange") {
        return TFT_ORANGE;
    } else if (color == "greenyellow") {
        return TFT_GREENYELLOW;
    } else if (color == "pink") {
        return TFT_PINK;
    } else if (color == "brown") {
        return TFT_BROWN;
    } else if (color == "gold") {
        return TFT_GOLD;
    } else if (color == "silver") {
        return TFT_SILVER;
    } else if (color == "skyblue") {
        return TFT_SKYBLUE;
    } else if (color == "vilolet") {
        return TFT_VIOLET;
    } else {
        Serial.print("Invalid color: ");
        Serial.println(color);
        return TFT_BLACK;
    }
}

String Utils::formatFloat(float value, int8_t digits)
{
    char tmp[30] = {};
    dtostrf(value, 1, digits, tmp);
    return tmp;
}

int32_t Utils::stringToAlignment(String alignment) {
    alignment.toLowerCase();
    if (alignment.indexOf(" ") != -1) {
        alignment = alignment[0] + alignment.substring(alignment.indexOf(" ") + 1, 1);
    }
    alignment.replace(" ", "");
    if (alignment == "tl") {
        return TL_DATUM;
    } else if (alignment == "tc") {
        return TC_DATUM;
    } else if (alignment == "tr") {
        return TR_DATUM;
    } else if (alignment == "ml") {
        return ML_DATUM;
    } else if (alignment == "mc") {
        return MC_DATUM;
    } else if (alignment == "mr") {
        return MR_DATUM;
    } else if (alignment == "bl") {
        return BL_DATUM;
    } else if (alignment == "bc") {
        return BC_DATUM;
    } else if (alignment == "br") {
        return BR_DATUM;
    } else if (alignment == "cl") {
        return CL_DATUM;
    } else if (alignment == "cc") {
        return CC_DATUM;
    } else if (alignment == "cr") {
        return CR_DATUM;
    } else if (alignment == "bl") {
        return BL_DATUM;
    } else if (alignment == "bc") {
        return BC_DATUM;
    } else if (alignment == "br") {
        return BR_DATUM;
    } else if (alignment == "l") {
        return L_BASELINE;
    } else if (alignment == "c") {
        return C_BASELINE;
    } else if (alignment == "r") {
        return R_BASELINE;
    } else {
        return TL_DATUM;
    }
}

