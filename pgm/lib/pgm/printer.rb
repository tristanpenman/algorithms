# frozen_string_literal: true

module PGM
  #
  # Helper class for printing data in an easily readable ASCII format
  #
  # Example output:
  #
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Smoker    | Smoker_0    | Smoker_0    | Smoker_1    | Smoker_1    |
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Pollution | Pollution_0 | Pollution_1 | Pollution_0 | Pollution_1 |
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Cancer_0  | 0.03        | 0.05        | 0.001       | 0.02        |
  #   +-----------+-------------+-------------+-------------+-------------+
  #   | Cancer_1  | 0.97        | 0.95        | 0.999       | 0.98        |
  #   +-----------+-------------+-------------+-------------+-------------+
  #
  class Printer
    class << self
      def print(rows)
        # calculate column widths
        num_columns = rows[0].length
        column_widths = (0...num_columns).map { |col| max_width(rows, col) }

        # write table
        div = write_divider(column_widths)
        rows.flat_map { |row| [write_row(column_widths, row), div] }
            .unshift(div)
            .join("\n")
      end

      private

      def max_width(rows, column)
        rows.map { |row| row[column].length }.max
      end

      def write_divider(column_widths)
        "+-#{column_widths.map { |g| '-' * g }.join('-+-')}-+"
      end

      def write_row(column_widths, values)
        "| #{values.map.with_index { |value, i| value.ljust(column_widths[i]) }.join(' | ')} |"
      end
    end
  end
end
