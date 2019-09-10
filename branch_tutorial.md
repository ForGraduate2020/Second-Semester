# Git Branch를 알아보자
- - - -
## Branch
**가지**라는 뜻으로 Git에서 작업흐름을 나눌 때 사용하는 용어이다. 흐름을 한 줄기만 가지고 갈 경우 여러사람이 작업할 때 프로젝트가 꼬일 우려가 있으므로 작업영역과 단위에 맞춰서 **Branch**를 나누는것이 좋을 것이다.


## 생성
git branch 명령으로 branch를 생성할 수 있다. 생성된 브랜치는 이전에 있던 branch와 동일한 상태이며 해당 branch에서 작업을 이어갈 수 있다.
```git
> git branch BRANCH_NAME
```


## 확인
현재 내가 있는 branch를 확인하기 위해서는 git branch 명령으로 확인한다. \*로 표시된 branch가 현재 내가 있는 branch이다.
```git
> git branch
*master
develop
```


## 이동(checkout)
여러 branch를 만들었다면 이곳저곳으로 옮겨다니면서 작업을 할 수 있다.
```git
> git checkout develop
> git branch
master
*develop
> git checkout master
> git branch
*master
develop
```


## 병합(merge)
branch를 나눠서 작업을 한 후 하나의 결과물로 병합해야한다. git merge 명령으로 병합할 수 있다. 현재 내가 있는 branch로 대상 branch가 병합된다.
따라서 아래 명령은 develop branch의 변경사항을 master branch로 병합하는 명령이다.
```git
> git branch
*master
develop
> git merge origin
```


## 활용
branch를 사용해서 작업흐름을 효율적으로 정리하는 기본적인 방법에 대해 알아본다.
배포버전인 master branch, 개발버전인 develop branch를 주축으로 운영한다고 가정한다.

1. 먼저 기능을 추가하고 싶다면 develop branch에서 자신의 local branch를 생성한다.
2. 충분한 테스트가 끝나고 기능이 완전해졌다고 판단하면 develop branch로 merge 한다.
3. develop에 팀원들의 변경사항들이 모여서 배포할 수준이 되면 master branch로 병합한다.

![](&&&SFLOCALFILEPATH&&&1.png)
