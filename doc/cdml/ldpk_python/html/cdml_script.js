// Diese Funktion wird aufgerufen, wenn man auf einen
// Indexeintrag dr�ckt. Dann wird das referenzierte Wort
// eingef�rbt.

function cdml_mark(s,k)
	{
	var i;
	var e = document.getElementsByTagName("cdml_indexitem");
	for(i = 0; i < e.length;i++)
		{
		e[i].style.backgroundColor = "inherit";
		}
	for(i = 0; i < e.length;i++)
		{
		if(e[i].getAttribute('keyword') == k)
			{
			e[i].style.backgroundColor = "#df8";
			}
		}
	window.location.hash = s;
	}

function cdml_mark_note(id)
	{
	var i;
	var e = document.getElementsByTagName("a");
	for(i = 0; i < e.length;i++)
		{
		e[i].style.backgroundColor = "inherit";
		}
	for(i = 0; i < e.length;i++)
		{
		if(e[i].getAttribute('href') == '#note_' + id)
			{
			e[i].style.backgroundColor = "#df8";
			}
		}
	window.location.hash = '#hir_note_' + id;
	}
