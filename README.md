## What is this?
This is a jekyll based-blog. Jekyll is a ruby-based framework for generating blogs. You write posts in markdown and jekyll takes care of the html generation automaticall. Jekyll is endorsed by Github and many github pages sites are made with Jekyll. There's tons of tutorials around, so this README will only provide instructions for getting started with jekyll and the post-writing workflow.

## Install Jekyll
You'll need:
* ruby
* gem package manager (`gem -v`)

```bash
gem install bundler
# installs all the dependencies of this project (including jekyll)
bundler update
jekyll -v
```

## How to write a post

```bash
jekyll serve
# open your browser to localhost:4000, you should see the site

cp _posts/2016-12-10-December-Progress.markdown _posts/2017-01-01-NewYearsResolutions.markdown
vim _posts/2017-01-01-NewYearsResolutions.markdown

# any edits you make will immediately be picked up and updated in your browser session
```