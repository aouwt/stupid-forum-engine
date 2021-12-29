void renderpost (post* p) {
	printf ("<p><h1><a href=\"%s%i%s\">%s</a></h1> by <a href=\"%s%s%s\">%s</a></p>",
		POST_URL_BEGIN, p -> id, POST_URL_END,
		p -> title,
		AUTHOR_URL_BEGIN, p -> author -> name, AUTHOR_URL_END,
		p -> author -> fname
	);

	{	char date [30];
		datestr (p -> date, &date);
		printf ("<i>%s</i>", date);
	}

	printf ("<p>%s</p>", p -> text);
}
