# csvp

One of the most common interfaces to business logic and non-technical users is a spreadsheet. When living in a fast paced data driven world, there is almost always a Project Manager, Account Manager, or some other stakeholder who has a spreadsheet with the information outlining some, if not all, of the data (and a decent amount of time, the structure) that should be used. Sometimes the spreadsheet is a nice outline for a database prototype, but once the connection to a spreadsheet/business logic is severed, we are forced to rely on the implementing engineer's, and in a perfect world code reviewer's, understanding of the requirements.

Once something is lost from the original concept deemed to be important, the thought should be WTF...why...its in the spreadsheet? does ALL the data provided not matter? If that question cannot be answered by a combination of the implementing engineer and/or code reviewer, how can we be sure it's implemented correctly, and ultimately make sure the change is not to be approved for promotion to production?

Understanding the problem is still crucial, and making an effort to maintain that connection in a way that forces inquisition into what may be missing/not add up/cause confusion is a nice first step towards a prototype and making the picture clearer.

## Usage

1. `make`
2. `./csvp -i ./path/to/relative/file.csv -o ./path/to/relative/output/file.json`

### flags

#### `-i` - the input csv file.
#### `-o` - the output file name with extension of `json`.
