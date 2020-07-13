#
# Problem B - Containers
#
# Containers arrive at a port in the order specified. The containers must be stacked such
# that a series of ships, each corresponding to a different letter in the input, can be
# loaded by removing containers from the tops of stacks. Containers may only be loaded from
# the top of a stack, and ships arrive in alpabetical order.
#
# This program outputs the minimum number of stacks required.
#
# Sample input:
#  A
#  CBACBACBACBACBA
#  CCCCBBBBAAAA
#  ACMICPC
#  ^D
#
# Sample output:
#  A: 1
#  CBACBACBACBACBA: 3
#  CCCCBBBBAAAA: 1
#  ACMICPC: 4
#
# Time taken: about 3 hours off and on
#
# Issues:
#  Started with a different tactic, in that I took notes and experimented without writing
#  code first. This worked pretty well since my first approach which was to try to find
#  the solution through some clever string manipulation (e.g. edit distance) did not seem
#  to be effective. After that, a search-based solution seemed like the best way to solve
#  this problem. I started with breadth-first search, but ran into performance problems.
#  Changing to a uniform-cost search (using a priority queue) appears to have resolved
#  these issues for the sample input.
#

import sys

from Queue import Queue, PriorityQueue

class State:

  @staticmethod
  def initial(containers):
    """ Create an initial state using the specified containers string """
    state = State()
    state.stacks = [[containers[0]]]
    state.containers = containers[1:]
    return state

  def copy(self):
    """ Make a copy of the current state """
    state = State()
    state.containers = self.containers
    state.stacks = []
    for stack in self.stacks:
      newStack = []
      for stackItem in stack:
        newStack.append(stackItem)
      state.stacks.append(newStack)
    return state

  def successors(self):
    """ Return a list of successors for the current state """
    successors = []

    # Only generate successors if there are more containers
    if len(self.containers) > 0:

      # Create a successor for each existing stack
      for n in range(0, len(self.stacks)):
        if ord(self.containers[0]) <= ord(self.stacks[n][-1]):
          newState = self.copy()
          newState.stacks[n].append(newState.containers[0])
          newState.containers = newState.containers[1:]
          successors.append(newState)

      # Create a successor using a new stack
      newState = self.copy()
      newState.stacks.append([newState.containers[0]])
      newState.containers = newState.containers[1:]
      successors.append(newState)

    return successors


for line in sys.stdin:
  line = line.rstrip().upper()

  # Create initial state using first container
  states = PriorityQueue()
  states.put((1, State.initial(line.rstrip().upper())))

  # Perform breadth first search
  while not states.empty():
    state = states.get()[1]
    if len(state.containers) == 0:
      # Found solution
      print(line + ": " + str(len(state.stacks)))
      break
    else:
      # Haven't found solution yet
      for successor in state.successors():
        states.put((len(successor.stacks), successor))

