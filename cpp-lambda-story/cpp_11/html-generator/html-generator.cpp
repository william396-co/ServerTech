#include <iostream>
#include <string>

void ValidateHTML( std::string const & )
{
}

std::string BuildAHref( std::string const & link, std::string const & text )
{
    const std::string html = [&link, &text] {
        auto const & inText = text.empty() ? link : text;
        return "<a href = \" " + link = " \">" + inText + "</a>";
    }();

    return html;
}

int main()
{

    try {
        const auto ahref = BuildAHref( "www.leanpub.com", "Leanpub Store" );
        std::cout << ahref << "\n";
    } catch ( ... ) {
        std::cout << "bad format..."
                  << "\n";
    }

    return 0;
}
