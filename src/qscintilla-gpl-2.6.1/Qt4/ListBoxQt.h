// This defines the specialisation of QListBox that handles the Scintilla
// double-click callback.
//
// Copyright (c) 2011 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public
// License versions 2.0 or 3.0 as published by the Free Software
// Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
// included in the packaging of this file.  Alternatively you may (at
// your option) use any later version of the GNU General Public
// License if such license has been publicly approved by Riverbank
// Computing Limited (or its successors, if any) and the KDE Free Qt
// Foundation. In addition, as a special exception, Riverbank gives you
// certain additional rights. These rights are described in the Riverbank
// GPL Exception version 1.1, which can be found in the file
// GPL_EXCEPTION.txt in this package.
// 
// If you are unsure which license is appropriate for your use, please
// contact the sales department at sales@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include <qmap.h>
#include <qpixmap.h>
#include <qstring.h>

#include "SciNamespace.h"

#include "Platform.h"


class QsciSciListBox;


// This is an internal class but it is referenced by a public class so it has
// to have a Qsci prefix rather than being put in the Scintilla namespace
// which would mean exposing the SCI_NAMESPACE mechanism).
class QsciListBoxQt : public QSCI_SCI_NAMESPACE(ListBox)
{
public:
    QsciListBoxQt();

    QSCI_SCI_NAMESPACE(CallBackAction) cb_action;
    void *cb_data;

    virtual void SetFont(QSCI_SCI_NAMESPACE(Font) &font);
    virtual void Create(QSCI_SCI_NAMESPACE(Window) &parent, int,
            QSCI_SCI_NAMESPACE(Point), int, bool unicodeMode, int);
    virtual void SetAverageCharWidth(int);
    virtual void SetVisibleRows(int);
    virtual int GetVisibleRows() const;
    virtual QSCI_SCI_NAMESPACE(PRectangle) GetDesiredRect();
    virtual int CaretFromEdge();
    virtual void Clear();
    virtual void Append(char *s, int type = -1);
    virtual int Length();
    virtual void Select(int n);
    virtual int GetSelection();
    virtual int Find(const char *prefix);
    virtual void GetValue(int n, char *value, int len);
    virtual void Sort();
    virtual void RegisterImage(int type, const char *xpm_data);
    virtual void RegisterRGBAImage(int type, int width, int height,
            const unsigned char *pixelsImage);
    virtual void ClearRegisteredImages();
    virtual void SetDoubleClickAction(
            QSCI_SCI_NAMESPACE(CallBackAction) action, void *data);
    virtual void SetList(const char *list, char separator, char typesep);

private:
    QsciSciListBox *slb;
    int visible_rows;
    bool utf8;

    typedef QMap<int, QPixmap> xpmMap;
    xpmMap xset;
};
