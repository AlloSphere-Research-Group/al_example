# Git

We use git because we don't want to ever lose our work---Git makes it nearly impossible to delete or lose anything. We want a way to track our work as it progresses, make backups, and save versions for demos and sharing. When we collaborate with others, we find we need a way to work separately, then merge our work together. Git does all this and more. Git is "hot". It's a technology with a lot of momentum (see [Github](http://github.com), [Bitbucket](https://bitbucket.org), etc.).

## Best tutorials and references

_The_ [Git Book](https://git-scm.com/book/en/v2) is a terrific resource. It's the official reference for Git. [StackOverflow](http://stackoverflow.com) is great for finding solutions to specific challenges that commonly arise for new Git users. If you google "how do i ____ with git?", you will probably get some StackOverflow hits. Also, Git has [man pages](https://en.wikipedia.org/wiki/Man_page) where you can read about how each git command works. If you want to know about `git init`, say `man git-init` in the terminal. If you want to know about `git pull`, say `man git-pull`.

Here's a few tutorials that are not bad:

- [Got 15 minutes and want to learn Git?](https://try.github.io/levels/1/challenges/1)
- [gittutorial - A tutorial introduction to Git](http://git-scm.com/docs/gittutorial)
- [Getting Git Right](https://www.atlassian.com/git)

## Non-Command Line Interfaces

We use git on the command line, but there are graphical/desktop applications for driving git. These are great for browsing, searching, and investigating problems in git repos. Even if you stick with the command line version of git (which I recommend), choose one or more of these programs, download, install, and look through your git repo.

- [gitk](https://git-scm.com/docs/gitk) (Linux & OSX) is the official GUI frontend for git.
- [GitX](http://rowanj.github.io/gitx) (OSX) is an excellent fork of gitk for OSX.
- [SourceTree](https://www.sourcetreeapp.com) (Windows & OSX) is the nicest git frontend I have seen.
- [TortoiseGit](https://tortoisegit.org) (Windows) integrates into Windows.

## Useful, common commands

	git status

...is the the command you should execute most often. It describes the current state of the current repo. 

	git log
	
...to see the history of changes to the repo or to (a) specific file(s).

	git log -- path/to/some/file.cpp

...will show you the history of a particular file.

	git whatchanged

...is like `git log` (it shows you the commit history) but, it will show which files changed.

	git diff

...shows the differences between specific versions of files. If you've modified any files tracked by Git since the last commit, `git diff` will show those changes in [diff unified format](https://en.wikipedia.org/wiki/Diff_utility#Unified_format) which is a machine (and human) readable way to describe changes to text files.

     git diff ae6c4357b52a06f366a3deb6b9b2f3c1610ad15c

...to see it differences between the current commit and commit ae6c4357b52a06f366a3deb6b9b2f3c1610ad15c. This long sequence of [hexidecimal](https://en.wikipedia.org/wiki/Hexadecimal) characters is a "commit hash"---Its a unique identifier that serves as the name of a commit.


## "Saving" your changes

In Git terms, "Saving" the changes you made locally to the "upstream" server is called "pushing". Before you can `git push`, you must `git pull`, `git add somefile.cpp`, and `git commit -m 'some descriptive message'`. _Then_ you can `git push`.

1. Go to your personal student repository

		cd student/tom.waits

2. Add the files (tells Git that you want them to be part of your next commit)

		git add myfile.cpp 

3. Commit the files (store these particular versions in the historical record of
your local repo). This requires a commit message that will be useful later as you
look at logs trying to answer questions about the history of your files such as 
"what did I do to break my program?"

		git commit -m "A precise message about this particular commit that will later help me"


4. Push the files (copies the commit(s) from your local repo to the "upstream" server.)

		git push


## Undoing mistakes

1. If a file is not already committed (locally), you may

		git checkout -- path/to/some/file.cpp

	(The "--" signals Git that it should interpret everything after the "--" as a list of file names.) Or, you may

		git reset --hard

	("hard" deletes all local changes since the last commit---This cannot be undone.)



2. If a file is committed locally

		git reset --soft HEAD~

	"--soft" requires there to be no local changes and allows you to go back to a previous version.

	...will revert to HEAD~ which means "the commit one previous to HEAD". HEAD is the current commit and ~ means "1 previous". There's also HEAD~2, HEAD~3, etc. You'd never say `git reset --soft HEAD` in this context because that's the version you already have.


3. If your changes are already pushed to the repository, then people may have already checked out your changes and possibly pushed new versions incorporating the changes you want to undo. Git won't let you commit something based on a previous source. You may take these steps to undo your changes...

	+ Use `git diff ...` to get a patch that converts the bad current version to the good previous version
	+ `git checkout ...` the current bad version (which may now have other modifications)
	+ Use `git apply ...` to apply the patch
	+ Commit and push: `git commit -m ...` and `git push`


## Merging

Git merges automatically if it can, i.e., if all changes occur on different lines. The result of an automatic merge may still not make semantic sense. If there are multiple changes on the same line(s) then it's a "conflict", and you have to apply some thought to incorporate the best of both versions.

Cool trick to clean up messages in git. Instead of pulling (fetching and merging) with

	git pull

You can instead "rebase" (stash diffs, pull, then reapply diffs as a local merge) with

	git pull --rebase

### Dealing with merge conflicts:

You've commited local changes with `git commit`. You tried to push but you can't because you're out of date, so you pull. This pull attempts to merge the upstream's changes with your local changes, possibly finding a merge conflict. (`git pull` is short for `git fetch` plus `git merge`.)

You will see a message if there's a conflict; also later you can see whether there's a conflict by typing

    git status

Also if there's a conflict you'll see something like ">>>>>> theirs" type stuff in your file, which you must address either with a text editor or with a conflict tool ("git mergetool" will give a list of them).

Finish addressing the conflicts. You should now build and test (to make sure both auto-merge and your manual conflict merge worked).

Now just "git add" the file again; git knows it gave you the file in a conflicted state (and that it warned you about the problem) so it now assumes you have resolved the conflict.  

Now just "git push" the file again; since git knows that your current version of the file is based on the current upstream version it lets you push. Unless while you were resolving the conflict somebody else pushed yet another conflicting version upstream, in which case you have
to go back to "git pull", possibly another manual merge, etc.


## Tagging

	git tag -a 'v1.2, Best Demo'

...will "tag" the current commit the the string "v1.2, Best Demo". You can also give extra information about tag with -m.


